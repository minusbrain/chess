#include "rules.h"

#include <base/improve_containers.h>

#include <cassert>
#include <iostream>

#include "bench.h"
#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "fmt/core.h"
#include "move.h"
#include "move_debug.h"
#include "piece_rules.h"
#include "types.h"

static std::map<Piece, std::function<std::vector<Move>(const Board& board, ChessPieceOnField pieceOnField)>> pieceRules = {
    {Piece::PAWN, PawnRules::getPotentialMoves},     {Piece::BISHOP, BishopRules::getPotentialMoves},
    {Piece::KNIGHT, KnightRules::getPotentialMoves}, {Piece::ROOK, RookRules::getPotentialMoves},
    {Piece::QUEEN, QueenRules::getPotentialMoves},   {Piece::KING, KingRules::getPotentialMoves},
    {Piece::DECOY, DecoyRules::getPotentialMoves}};

inline constexpr Color getOppositeColor(Color color) { return (color == Color::WHITE ? Color::BLACK : Color::WHITE); }

bool ChessRules::isCheck(const Board& board) {
    std::optional<ChessField> kingFieldOpt = board.findFirstPiece([&board](ChessPiece cp) {
        return cp == ChessPiece{board.whosTurnIsIt(), Piece::KING};
    });
    assert(kingFieldOpt.has_value());
    ChessField kingField = kingFieldOpt.value();

    return isFieldCoveredByColor(board, kingField, getOppositeColor(board.whosTurnIsIt()));
};

bool ChessRules::isCheckMate(const Board& board, bool checkHint) {
    if (checkHint || isCheck(board)) {
        auto moves = getAllValidMoves(board, false);
        if (moves.size() == 0) {
            return true;
        }
    }
    return false;
};

bool ChessRules::isStaleMate(const Board& board, bool checkHint) {
    if (!checkHint || !isCheck(board)) {
        auto moves = getAllValidMoves(board, false);
        if (moves.size() == 0) {
            return true;
        }
    }
    return false;
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

bool ChessRules::wouldMoveSelfIntoCheck(const Board& board, const Move& move) {
    Color color = board.whosTurnIsIt();

    Board postMoveBoard(board);
    applyMove(postMoveBoard, move);
    std::optional<ChessField> kingFieldOpt = postMoveBoard.findFirstPiece([color](ChessPiece cp) {
        return cp == ChessPiece{color, Piece::KING};
    });
    assert(kingFieldOpt.has_value());
    ChessField kingField = kingFieldOpt.value();

    return isFieldCoveredByColor(postMoveBoard, kingField, postMoveBoard.whosTurnIsIt());
};

std::vector<Move> ChessRules::getAllValidMoves(const Board& board, bool annotate) {
    std::vector<Move> validMoves;
    std::vector<Move> potentialMoves = getAllPotentialMoves(board);

    for (auto potentialMove : potentialMoves) {
        if (ChessRules::isMoveLegal(board, potentialMove)) validMoves.push_back(potentialMove);
    }

    if (annotate) annotateMoves(board, validMoves);

    return validMoves;
}

void ChessRules::annotateMoves(const Board& board, std::vector<Move>& moves) {
    for (auto& move : moves) {
        // std::cout << "\nApplying " << move << " to " << board << std::flush;
        Board resultingBoard(board);
        applyMove(resultingBoard, move);
        bool check = isCheck(resultingBoard);

        if (isCheckMate(resultingBoard, check)) {
            // std::cout << "\nMove " << move << " is check mate!" << std::flush;
            move.addModifier(MoveModifier::CHECK_MATE);
        } else if (isStaleMate(resultingBoard, check)) {
            // std::cout << "\nMove " << move << " is stale mate!" << std::flush;
            move.addModifier(MoveModifier::STALE_MATE);
        } else if (check) {
            // std::cout << "\nMove " << move << " is check!" << std::flush;
            move.addModifier(MoveModifier::CHECK);
        }
    }
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

    return pieceRules[piece](board, pieceOnField);
}

bool ChessRules::isCastlingLegal(const Board& board, const Move& potentialMove) {
    Color movingColor = std::get<ColorIdx>(potentialMove.getChessPiece());
    ChessRank castlingRank = (movingColor == Color::WHITE ? 1 : 8);

    if (potentialMove.hasModifier(MoveModifier::CASTLING_LONG)) {
        return !isFieldCoveredByColor(board, ChessField{D, castlingRank}, getOppositeColor(movingColor));

    } else if (potentialMove.hasModifier(MoveModifier::CASTLING_SHORT)) {
        return !isFieldCoveredByColor(board, ChessField{F, castlingRank}, getOppositeColor(movingColor));
    }

    return true;
}

// Expects that the provided move follows the basic movement rules. This just checks if
// castling is legal and if the move would create a check for the other side.
bool ChessRules::isMoveLegal(const Board& board, const Move& potentialMove) {
    if (potentialMove.hasModifier(MoveModifier::CASTLING_LONG) || potentialMove.hasModifier(MoveModifier::CASTLING_SHORT)) {
        if (!isCastlingLegal(board, potentialMove)) {
            return false;
        }
    }
    if (potentialMove.hasModifier(MoveModifier::CAPTURE)) {
        if (board.getPieceOnField(potentialMove.getEndField()).has_value() &&
            std::get<PieceIdx>(board.getPieceOnField(potentialMove.getEndField()).value()) == Piece::KING)
            return false;
    }
    return !wouldMoveSelfIntoCheck(board, potentialMove);
}

bool ChessRules::applyMove(Board& board, const Move& move, bool assertLegal) {
    auto cp = move.getChessPiece();
    Color movingColor = std::get<ColorIdx>(cp);
    assert(movingColor == board.whosTurnIsIt());
    auto sf = move.getStartField();
    auto ef = move.getEndField();
    assert(BoardHelper::isInBounds(sf));
    assert(BoardHelper::isInBounds(ef));

    auto startPiece = board.getPieceOnField(sf);
    auto endPiece = board.getPieceOnField(ef);

    bool pawnOrCaptureMove = false;

    if (assertLegal && board.isLegalPosition() == false) {
        fmt::print("Input of applyMove() illegal: {} Move: {}\n", board.getFENString(), move);
        assert(false);
    }

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
        auto epTargetOpt = board.getPieceOnField(epFieldOpt.value());
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
            if (board.getPieceOnField({B, castlingRank}).has_value() || board.getPieceOnField({C, castlingRank}).has_value() ||
                board.getPieceOnField({D, castlingRank}).has_value()) {
                return false;
            }

            if (!board.getPieceOnField({A, castlingRank}).has_value() ||
                board.getPieceOnField({A, castlingRank}).value() != ChessPiece{movingColor, Piece::ROOK}) {
                return false;
            }

            if (!board.getPieceOnField({E, castlingRank}).has_value() ||
                board.getPieceOnField({E, castlingRank}).value() != ChessPiece{movingColor, Piece::KING}) {
                return false;
            }
            board.clearField({A, castlingRank});
            board.setField({D, castlingRank}, {movingColor, Piece::ROOK});
            if (movingColor == Color::WHITE)
                board.unsetCastling(Board::Castling::WHITE_LONG);
            else
                board.unsetCastling(Board::Castling::BLACK_LONG);
        } else {
            if (board.getPieceOnField({F, castlingRank}).has_value() || board.getPieceOnField({G, castlingRank}).has_value()) {
                return false;
            }

            if (!board.getPieceOnField({H, castlingRank}).has_value() ||
                board.getPieceOnField({H, castlingRank}).value() != ChessPiece{movingColor, Piece::ROOK}) {
                return false;
            }

            if (!board.getPieceOnField({E, castlingRank}).has_value() ||
                board.getPieceOnField({E, castlingRank}).value() != ChessPiece{movingColor, Piece::KING}) {
                return false;
            }
            board.clearField({H, castlingRank});
            board.setField({F, castlingRank}, {movingColor, Piece::ROOK});
            if (movingColor == Color::WHITE)
                board.unsetCastling(Board::Castling::WHITE_SHORT);
            else
                board.unsetCastling(Board::Castling::BLACK_SHORT);
        }
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

    if (move.hasModifier(MoveModifier::CAPTURE)) {
        pawnOrCaptureMove = true;
        if (ef == ChessField{A, 1}) {
            board.unsetCastling(Board::Castling::WHITE_LONG);
        } else if (ef == ChessField{H, 1}) {
            board.unsetCastling(Board::Castling::WHITE_SHORT);
        } else if (ef == ChessField{A, 8}) {
            board.unsetCastling(Board::Castling::BLACK_LONG);
        } else if (ef == ChessField{H, 8}) {
            board.unsetCastling(Board::Castling::BLACK_SHORT);
        }
    }

    if (piece == Piece::PAWN) {
        pawnOrCaptureMove = true;
        if (move.hasModifier(MoveModifier::PROMOTE_QUEEN)) {
            std::get<PieceIdx>(cp) = Piece::QUEEN;
        } else if (move.hasModifier(MoveModifier::PROMOTE_BISHOP)) {
            std::get<PieceIdx>(cp) = Piece::BISHOP;
        } else if (move.hasModifier(MoveModifier::PROMOTE_KNIGHT)) {
            std::get<PieceIdx>(cp) = Piece::KNIGHT;
        } else if (move.hasModifier(MoveModifier::PROMOTE_ROOK)) {
            std::get<PieceIdx>(cp) = Piece::ROOK;
        }
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
    board.setTurn(getOppositeColor(movingColor));

    if (movingColor == Color::BLACK) {
        board.incrementFullMove();
    }

    if (pawnOrCaptureMove) {
        board.resetHalfMoveClock();
    } else {
        board.incrementHalfMoveClock();
    }

    if (assertLegal && board.isLegalPosition() == false) {
        fmt::print("Output of applyMove() illegal: {} Move: {}\n", board.getFENString(), move);
        assert(false);
    }

    return true;
}

// IDEA: Return reason for illegal verdict
Legality ChessRules::determineBoardPositionLegality(Board& board) {
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
        if (!board.getPieceOnField({A, 1}).has_value() || board.getPieceOnField({A, 1}).value() != ChessPiece{Color::WHITE, Piece::ROOK})
            return Legality::ILLEGAL;
        if (!board.getPieceOnField({E, 1}).has_value() || board.getPieceOnField({E, 1}).value() != ChessPiece{Color::WHITE, Piece::KING})
            return Legality::ILLEGAL;
    }

    if (board.canCastle(Board::Castling::WHITE_SHORT)) {
        if (!board.getPieceOnField({H, 1}).has_value() || board.getPieceOnField({H, 1}).value() != ChessPiece{Color::WHITE, Piece::ROOK})
            return Legality::ILLEGAL;
        if (!board.getPieceOnField({E, 1}).has_value() || board.getPieceOnField({E, 1}).value() != ChessPiece{Color::WHITE, Piece::KING})
            return Legality::ILLEGAL;
    }

    if (board.canCastle(Board::Castling::BLACK_LONG)) {
        if (!board.getPieceOnField({A, 8}).has_value() || board.getPieceOnField({A, 8}).value() != ChessPiece{Color::BLACK, Piece::ROOK})
            return Legality::ILLEGAL;
        if (!board.getPieceOnField({E, 8}).has_value() || board.getPieceOnField({E, 8}).value() != ChessPiece{Color::BLACK, Piece::KING})
            return Legality::ILLEGAL;
    }

    if (board.canCastle(Board::Castling::BLACK_SHORT)) {
        if (!board.getPieceOnField({H, 8}).has_value() || board.getPieceOnField({H, 8}).value() != ChessPiece{Color::BLACK, Piece::ROOK})
            return Legality::ILLEGAL;
        if (!board.getPieceOnField({E, 8}).has_value() || board.getPieceOnField({E, 8}).value() != ChessPiece{Color::BLACK, Piece::KING})
            return Legality::ILLEGAL;
    }

    if (board.hasEnPassantTarget()) {
        auto epField = board.getEnPassantTarget().value();
        ChessRank rank = std::get<ChessRankIdx>(epField);
        ChessFile file = std::get<ChessFileIdx>(epField);

        if (rank != 3 && rank != 6)
            return Legality::ILLEGAL;
        else if (rank == 3) {
            if (!board.getPieceOnField({file, 4}).has_value() ||
                board.getPieceOnField({file, 4}).value() != ChessPiece{Color::WHITE, Piece::PAWN})
                return Legality::ILLEGAL;
        } else {
            if (!board.getPieceOnField({file, 5}).has_value() ||
                board.getPieceOnField({file, 5}).value() != ChessPiece{Color::BLACK, Piece::PAWN})
                return Legality::ILLEGAL;
        }
    }

    auto whiteKing = board.findFirstPiece([](const ChessPiece& cp) { return cp == ChessPiece{Color::WHITE, Piece::KING}; });
    auto blackKing = board.findFirstPiece([](const ChessPiece& cp) { return cp == ChessPiece{Color::BLACK, Piece::KING}; });

    int fileDist = std::abs(std::get<ChessFileIdx>(whiteKing.value()) - std::get<ChessFileIdx>(blackKing.value()));
    int rankDist = std::abs(std::get<ChessRankIdx>(whiteKing.value()) - std::get<ChessRankIdx>(blackKing.value()));

    if (fileDist <= 1 && rankDist <= 1) return Legality::ILLEGAL;

    return Legality::LEGAL;
}
