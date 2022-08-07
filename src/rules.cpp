#include "rules.h"

#include <base/improve_containers.h>

#include <cassert>

#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "fmt/core.h"
#include "move.h"
#include "move_debug.h"
#include "piece_rules.h"
#include "types.h"

static std::map<Piece, ChessPieceRules*> pieceRules = {{Piece::PAWN, new PawnRules()},     {Piece::BISHOP, new BishopRules()},
                                                       {Piece::KNIGHT, new KnightRules()}, {Piece::ROOK, new RookRules()},
                                                       {Piece::QUEEN, new QueenRules()},   {Piece::KING, new KingRules()},
                                                       {Piece::DECOY, new DecoyRules()}};

Color getOppositeColor(Color color) { return (color == Color::WHITE ? Color::BLACK : Color::WHITE); }

bool ChessRules::isCheck(const Board& board) {
    std::optional<ChessField> kingFieldOpt = board.findFirstPiece([&board](ChessPiece cp) {
        return cp == ChessPiece{board.whosTurnIsIt(), Piece::KING};
    });
    assert(kingFieldOpt.has_value());
    ChessField kingField = kingFieldOpt.value();

    Board otherPlayerBoard(board);
    otherPlayerBoard.setTurn(getOppositeColor(board.whosTurnIsIt()));

    return isFieldCoveredByColor(otherPlayerBoard, kingField, board.whosTurnIsIt());
};

bool ChessRules::isFieldCoveredByColor(const Board& board, const ChessField& field, Color color) {
    Board tempBoard(board);
    Color oppositeColor = getOppositeColor(color);

    tempBoard.setField(field, {oppositeColor, Piece::DECOY});
    tempBoard.setTurn(color);
    std::vector<Move> potentialMoves = getAllPotentialMoves(tempBoard);
    int numOfCaptureMoves = base::count_if(
        potentialMoves, [&field](const Move& move) { return (move.getEndField() == field && move.hasModifier(MoveModifier::CAPTURE)); });

    return (numOfCaptureMoves > 0);
}

bool ChessRules::isCheckMate(const Board& board) {
    // Todo Properly implement. We need some kind of caching for Board -> Moves -> Resulting boards
    if (!isCheck(board)) {
        return false;
    }

    return false;
};

bool ChessRules::wouldBeCheck(const Board& board, const Move& move) {
    // fmt::print("WouldBeCheck move {} to board \n{}", move, board);
    Color color = board.whosTurnIsIt();

    Board otherPlayerBoard(board);
    applyMove(otherPlayerBoard, move);
    std::optional<ChessField> kingFieldOpt = otherPlayerBoard.findFirstPiece([color](ChessPiece cp) {
        return cp == ChessPiece{color, Piece::KING};
    });
    assert(kingFieldOpt.has_value());
    ChessField kingField = kingFieldOpt.value();

    return isFieldCoveredByColor(otherPlayerBoard, kingField, otherPlayerBoard.whosTurnIsIt());
};

bool ChessRules::wouldBeCheckMate(const Board& board, const Move& move) {
    // Todo Properly implement. We need some kind of caching for Board -> Moves -> Resulting boards
    if (!wouldBeCheck(board, move)) {
        return false;
    }
    return false;
};

std::vector<Move> ChessRules::getAllValidMoves(const Board& board, IgnoreCheck ignoreCheck) {
    std::vector<Move> validMoves;

    auto potentialMoves = getAllPotentialMoves(board);

    for (auto potentialMove : potentialMoves) {
        if (ChessRules::isMoveLegal(board, potentialMove, ignoreCheck)) validMoves.push_back(potentialMove);
    }

    return validMoves;
}

std::vector<Move> ChessRules::getAllPotentialMoves(const Board& board) {
    std::vector<Move> potentialMoves;

    auto allPiecesOfRequestedColor = board.getAllPieces(board.whosTurnIsIt());

    for (auto pieceOnField : allPiecesOfRequestedColor) {
        auto potentialMovesOfSinglePiece = ChessRules::getPotentialMoves(board, pieceOnField);
        base::push_back(potentialMoves, potentialMovesOfSinglePiece);
    };

    return potentialMoves;
}

// TODO: Refactor to use polymorphic approach to get movement options per piece
std::vector<Move> ChessRules::getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) {
    ChessPiece cp = std::get<ChessPieceIdx>(pieceOnField);
    Piece piece = std::get<PieceIdx>(cp);

    return pieceRules[piece]->getPotentialMoves(board, pieceOnField);
}

bool ChessRules::isCastlingLegal(const Board& board, const Move& potentialMove) {
    // Todo. Fix / Implement by using      numOfPotentialMovesCoveringField(const Board &board, ChessField field)
    Color movingColor = std::get<ColorIdx>(potentialMove.getChessPiece());
    ChessRank castlingRank = (movingColor == Color::WHITE ? 1 : 8);

    if (potentialMove.hasModifier(MoveModifier::CASTLING_LONG)) {
        if (board.getField({B, castlingRank}).has_value() || board.getField({C, castlingRank}).has_value() ||
            board.getField({D, castlingRank}).has_value()) {
            return false;
        }
        return !isFieldCoveredByColor(board, ChessField{D, castlingRank}, getOppositeColor(movingColor));

    } else if (potentialMove.hasModifier(MoveModifier::CASTLING_SHORT)) {
        if (board.getField({F, castlingRank}).has_value() || board.getField({G, castlingRank}).has_value()) {
            return false;
        }
        return !isFieldCoveredByColor(board, ChessField{F, castlingRank}, getOppositeColor(movingColor));
    }

    // If the final position of the king is covered is checked on higher level, so we do not check it here again.

    return true;
}

bool ChessRules::isMoveLegal(const Board& board, const Move& potentialMove, IgnoreCheck ignoreCheck) {
    if (potentialMove.hasModifier(MoveModifier::CASTLING_LONG) || potentialMove.hasModifier(MoveModifier::CASTLING_SHORT)) {
        if (!isCastlingLegal(board, potentialMove)) {
            return false;
        }
    }
    return ignoreCheck == IgnoreCheck::YES || !wouldBeCheck(board, potentialMove);
}

// Todo applyMove in rules. set / unset EnPassant ...
bool ChessRules::applyMove(Board& board, const Move& move) {
    assert(determineBoardPositionLegality(board) == Legality::LEGAL);
    auto cp = move.getChessPiece();
    Color movingColor = std::get<ColorIdx>(cp);
    auto sf = move.getStartField();
    auto ef = move.getEndField();
    assert(BoardHelper::isInBounds(sf));
    assert(BoardHelper::isInBounds(ef));

    auto startPiece = board.getField(sf);
    auto endPiece = board.getField(ef);

    if (!startPiece.has_value() || startPiece.value() != cp) {
        return false;
    }
    if (endPiece.has_value() && (!move.hasModifier(MoveModifier::CAPTURE) || std::get<ColorIdx>(endPiece.value()) == movingColor)) {
        return false;
    }
    if (move.hasModifier(MoveModifier::EN_PASSANT)) {
        if (std::get<PieceIdx>(cp) != Piece::PAWN || !move.hasModifier(MoveModifier::CAPTURE)) {
            return false;
        }
        auto epFieldOpt = BoardHelper::determineEnPassantCaptureTarget(sf, ef);
        if (!epFieldOpt.has_value()) {
            return false;
        }
        auto epTargetOpt = board.getField(epFieldOpt.value());
        if (!epTargetOpt.has_value()) {
            return false;
        }
        auto epTarget = epTargetOpt.value();
        if (std::get<ColorIdx>(epTarget) == movingColor || std::get<PieceIdx>(epTarget) != Piece::PAWN) {
            return false;
        }
        board.clearField(epFieldOpt.value());
    } else if (move.hasModifier(MoveModifier::CASTLING_LONG) || move.hasModifier(MoveModifier::CASTLING_SHORT)) {
        ChessRank castlingRank = (movingColor == Color::WHITE ? 1 : 8);

        if (move.hasModifier(MoveModifier::CASTLING_LONG)) {
            if (board.getField({B, castlingRank}).has_value() || board.getField({C, castlingRank}).has_value() ||
                board.getField({D, castlingRank}).has_value()) {
                return false;
            }

            if (!board.getField({A, castlingRank}).has_value() ||
                board.getField({A, castlingRank}).value() != ChessPiece{movingColor, Piece::ROOK}) {
                return false;
            }

            if (!board.getField({E, castlingRank}).has_value() ||
                board.getField({E, castlingRank}).value() != ChessPiece{movingColor, Piece::KING}) {
                return false;
            }
            board.clearField({A, castlingRank});
            board.setField({D, castlingRank}, {movingColor, Piece::ROOK});
            if (movingColor == Color::WHITE)
                board.unsetCastling(Board::Castling::WHITE_LONG);
            else
                board.unsetCastling(Board::Castling::BLACK_LONG);
        } else {
            if (board.getField({F, castlingRank}).has_value() || board.getField({G, castlingRank}).has_value()) {
                return false;
            }

            if (!board.getField({H, castlingRank}).has_value() ||
                board.getField({H, castlingRank}).value() != ChessPiece{movingColor, Piece::ROOK}) {
                return false;
            }

            if (!board.getField({E, castlingRank}).has_value() ||
                board.getField({E, castlingRank}).value() != ChessPiece{movingColor, Piece::KING}) {
                return false;
            }
            board.clearField({H, castlingRank});
            board.setField({F, castlingRank}, {movingColor, Piece::ROOK});
            if (movingColor == Color::WHITE)
                board.unsetCastling(Board::Castling::WHITE_SHORT);
            else
                board.unsetCastling(Board::Castling::BLACK_SHORT);
        }
    } else if (move.hasModifier(MoveModifier::PROMOTE_QUEEN)) {
        std::get<PieceIdx>(cp) = Piece::QUEEN;
    } else if (move.hasModifier(MoveModifier::PROMOTE_BISHOP)) {
        std::get<PieceIdx>(cp) = Piece::BISHOP;
    } else if (move.hasModifier(MoveModifier::PROMOTE_KNIGHT)) {
        std::get<PieceIdx>(cp) = Piece::KNIGHT;
    } else if (move.hasModifier(MoveModifier::PROMOTE_ROOK)) {
        std::get<PieceIdx>(cp) = Piece::ROOK;
    }

    auto piece = std::get<PieceIdx>(cp);
    auto color = std::get<ColorIdx>(cp);

    if (piece == Piece::KING) {
        if (color == Color::WHITE) {
            board.unsetCastling(Board::Castling::WHITE_LONG);
            board.unsetCastling(Board::Castling::WHITE_SHORT);
        } else {
            board.unsetCastling(Board::Castling::BLACK_LONG);
            board.unsetCastling(Board::Castling::BLACK_SHORT);
        }
    } else if (piece == Piece::ROOK) {
        if (sf == ChessField{A, 1}) {
            board.unsetCastling(Board::Castling::WHITE_LONG);
        } else if (sf == ChessField{A, 8}) {
            board.unsetCastling(Board::Castling::BLACK_LONG);
        } else if (sf == ChessField{H, 1}) {
            board.unsetCastling(Board::Castling::WHITE_SHORT);
        } else if (sf == ChessField{H, 8}) {
            board.unsetCastling(Board::Castling::BLACK_SHORT);
        }
    }

    if (piece == Piece::PAWN) {
        int distance = std::abs(std::get<ChessRankIdx>(ef) - std::get<ChessRankIdx>(sf));
        if (distance == 2) {
            board.setEnPassantTarget(
                ChessField{std::get<ChessFileIdx>(sf), std::get<ChessRankIdx>(sf) + (movingColor == Color::WHITE ? 1 : -1)});
        } else {
            board.removeEnPassantTarget();
        }
    } else {
        board.removeEnPassantTarget();
    }

    board.clearField(sf);
    board.setField(ef, cp);
    board.setTurn(movingColor == Color::WHITE ? Color::BLACK : Color::WHITE);
    assert(determineBoardPositionLegality(board) == Legality::LEGAL);
    return true;
}

// IDEA: Return reason for illegal verdict
Legality ChessRules::determineBoardPositionLegality(Board& board) {
    if (board.getLegality() != Legality::UNDETERMINED) {
        return board.getLegality();
    }
    board.setLegality(Legality::ILLEGAL);

    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::WHITE, Piece::KING}; }) != 1)
        return Legality::ILLEGAL;
    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::BLACK, Piece::KING}; }) != 1)
        return Legality::ILLEGAL;

    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::WHITE, Piece::PAWN}; }) > 8)
        return Legality::ILLEGAL;
    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::BLACK, Piece::PAWN}; }) > 8)
        return Legality::ILLEGAL;

    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::WHITE, Piece::BISHOP}; }) > 10)
        return Legality::ILLEGAL;
    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::BLACK, Piece::BISHOP}; }) > 10)
        return Legality::ILLEGAL;

    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::WHITE, Piece::KNIGHT}; }) > 10)
        return Legality::ILLEGAL;
    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::BLACK, Piece::KNIGHT}; }) > 10)
        return Legality::ILLEGAL;

    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::WHITE, Piece::ROOK}; }) > 10)
        return Legality::ILLEGAL;
    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::BLACK, Piece::ROOK}; }) > 10)
        return Legality::ILLEGAL;

    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::WHITE, Piece::QUEEN}; }) > 9)
        return Legality::ILLEGAL;
    if (board.countAllPieces([](const ChessPiece& cp) { return cp == ChessPiece{Color::BLACK, Piece::QUEEN}; }) > 9)
        return Legality::ILLEGAL;

    if (board.countAllPieces([](const ChessPiece& cp) { return std::get<ColorIdx>(cp) == Color::WHITE; }) > 16) return Legality::ILLEGAL;
    if (board.countAllPieces([](const ChessPiece& cp) { return std::get<ColorIdx>(cp) == Color::BLACK; }) > 16) return Legality::ILLEGAL;

    if (board.canCastle(Board::Castling::WHITE_LONG)) {
        if (!board.getField({A, 1}).has_value() || board.getField({A, 1}).value() != ChessPiece{Color::WHITE, Piece::ROOK})
            return Legality::ILLEGAL;
        if (!board.getField({E, 1}).has_value() || board.getField({E, 1}).value() != ChessPiece{Color::WHITE, Piece::KING})
            return Legality::ILLEGAL;
    }

    if (board.canCastle(Board::Castling::WHITE_SHORT)) {
        if (!board.getField({H, 1}).has_value() || board.getField({H, 1}).value() != ChessPiece{Color::WHITE, Piece::ROOK})
            return Legality::ILLEGAL;
        if (!board.getField({E, 1}).has_value() || board.getField({E, 1}).value() != ChessPiece{Color::WHITE, Piece::KING})
            return Legality::ILLEGAL;
    }

    if (board.canCastle(Board::Castling::BLACK_LONG)) {
        if (!board.getField({A, 8}).has_value() || board.getField({A, 8}).value() != ChessPiece{Color::BLACK, Piece::ROOK})
            return Legality::ILLEGAL;
        if (!board.getField({E, 8}).has_value() || board.getField({E, 8}).value() != ChessPiece{Color::BLACK, Piece::KING})
            return Legality::ILLEGAL;
    }

    if (board.canCastle(Board::Castling::BLACK_SHORT)) {
        if (!board.getField({H, 8}).has_value() || board.getField({H, 8}).value() != ChessPiece{Color::BLACK, Piece::ROOK})
            return Legality::ILLEGAL;
        if (!board.getField({E, 8}).has_value() || board.getField({E, 8}).value() != ChessPiece{Color::BLACK, Piece::KING})
            return Legality::ILLEGAL;
    }

    if (board.hasEnPassantTarget()) {
        auto epField = board.getEnPassantTarget().value();
        ChessRank rank = std::get<ChessRankIdx>(epField);
        ChessFile file = std::get<ChessFileIdx>(epField);

        if (rank != 3 && rank != 6)
            return Legality::ILLEGAL;
        else if (rank == 3) {
            if (!board.getField({file, 4}).has_value() || board.getField({file, 4}).value() != ChessPiece{Color::WHITE, Piece::PAWN})
                return Legality::ILLEGAL;
        } else {
            if (!board.getField({file, 5}).has_value() || board.getField({file, 5}).value() != ChessPiece{Color::BLACK, Piece::PAWN})
                return Legality::ILLEGAL;
        }
    }

    auto whiteKing = board.findFirstPiece([](const ChessPiece& cp) { return cp == ChessPiece{Color::WHITE, Piece::KING}; });
    auto blackKing = board.findFirstPiece([](const ChessPiece& cp) { return cp == ChessPiece{Color::BLACK, Piece::KING}; });

    int fileDist = std::abs(std::get<ChessFileIdx>(whiteKing.value()) - std::get<ChessFileIdx>(blackKing.value()));
    int rankDist = std::abs(std::get<ChessRankIdx>(whiteKing.value()) - std::get<ChessRankIdx>(blackKing.value()));

    if (fileDist <= 1 && rankDist <= 1) return Legality::ILLEGAL;

    board.setLegality(Legality::LEGAL);

    return Legality::LEGAL;
}
