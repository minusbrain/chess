#include "rules.h"

#include <base/improve_containers.h>
#include <base/scope_guard.h>

#include <cassert>

#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "fmt/core.h"
#include "move.h"
#include "move_debug.h"
#include "types.h"

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

bool ChessRules::isFieldCoveredByColor(const Board& board, ChessField& field, Color color) {
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
// Checked for hidden recursion: NO
std::vector<Move> ChessRules::getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) {
    ChessPiece cp = std::get<ChessPieceIdx>(pieceOnField);
    Piece piece = std::get<PieceIdx>(cp);

    switch (piece) {
        case Piece::PAWN:
            return getPotentialPawnMoves(board, pieceOnField);
            break;
        case Piece::ROOK:
            return getPotentialRookMoves(board, pieceOnField);
            break;
        case Piece::KNIGHT:
            return getPotentialKnightMoves(board, pieceOnField);
            break;
        case Piece::BISHOP:
            return getPotentialBishopMoves(board, pieceOnField);
            break;
        case Piece::QUEEN:
            return getPotentialQueenMoves(board, pieceOnField);
            break;
        case Piece::KING:
            return getPotentialKingMoves(board, pieceOnField);
            break;
        case Piece::DECOY:
            return {};
            break;
    }
    return {};
}

// Checked for hidden recursion: NO
std::vector<Move> ChessRules::getPotentialPawnMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;

    ChessPiece cp = std::get<ChessPieceIdx>(pieceOnField);
    Color color = std::get<ColorIdx>(cp);
    ChessField currentField = std::get<ChessFieldIdx>(pieceOnField);
    ChessFile currentFile = std::get<ChessFileIdx>(currentField);
    ChessFile currentRank = std::get<ChessRankIdx>(currentField);

    assert(board.whosTurnIsIt() == color);

    int doubleMoveRank = (color == Color::WHITE ? 2 : 7);
    int direction = (color == Color::WHITE ? 1 : -1);

    ChessField potentialEndField = {currentFile, currentRank + direction};
    if (!board.getField(potentialEndField).has_value()) {
        Move singleStep = {cp, currentField, potentialEndField};

        if (std::get<ChessRankIdx>(potentialEndField) == 8 || std::get<ChessRankIdx>(potentialEndField) == 1) {
            // Single step onto last rank
            singleStep.addModifier(MoveModifier::PROMOTE_BISHOP);
            potentialMoves.push_back(singleStep);
            singleStep.clearModifiers();
            singleStep.addModifier(MoveModifier::PROMOTE_KNIGHT);
            potentialMoves.push_back(singleStep);
            singleStep.clearModifiers();
            singleStep.addModifier(MoveModifier::PROMOTE_ROOK);
            potentialMoves.push_back(singleStep);
            singleStep.clearModifiers();
            singleStep.addModifier(MoveModifier::PROMOTE_QUEEN);
            potentialMoves.push_back(singleStep);
        } else {
            // Single step somewhere
            potentialMoves.push_back(singleStep);
        }
        if (currentRank == doubleMoveRank) {
            // Double step from rank 2 or 7
            std::get<ChessRankIdx>(potentialEndField) = currentRank + (2 * direction);
            if (!board.getField(potentialEndField).has_value()) {
                Move doubleStep = {cp, currentField, potentialEndField};
                potentialMoves.push_back(doubleStep);
            }
        }
    }
    if (currentFile != A) {
        ChessField potentialCaptureTargetField = {currentFile - 1, currentRank + direction};
        auto potentialCaptureTarget = board.getField(potentialCaptureTargetField);
        if (potentialCaptureTarget.has_value() && std::get<ColorIdx>(potentialCaptureTarget.value()) != color) {
            Move captureMove = {cp, currentField, potentialCaptureTargetField};
            if (std::get<ChessRankIdx>(potentialCaptureTargetField) == 8 || std::get<ChessRankIdx>(potentialCaptureTargetField) == 1) {
                captureMove.addModifier(MoveModifier::CAPTURE);
                captureMove.addModifier(MoveModifier::PROMOTE_BISHOP);
                potentialMoves.push_back(captureMove);
                captureMove.clearModifiers();
                captureMove.addModifier(MoveModifier::CAPTURE);
                captureMove.addModifier(MoveModifier::PROMOTE_KNIGHT);
                potentialMoves.push_back(captureMove);
                captureMove.clearModifiers();
                captureMove.addModifier(MoveModifier::CAPTURE);
                captureMove.addModifier(MoveModifier::PROMOTE_ROOK);
                potentialMoves.push_back(captureMove);
                captureMove.clearModifiers();
                captureMove.addModifier(MoveModifier::CAPTURE);
                captureMove.addModifier(MoveModifier::PROMOTE_QUEEN);
                potentialMoves.push_back(captureMove);
            } else {
                captureMove.addModifier(MoveModifier::CAPTURE);
                potentialMoves.push_back(captureMove);
            }
        } else if (potentialCaptureTargetField == board.getEnPassantTarget()) {
            Move captureMove = {cp, currentField, potentialCaptureTargetField, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT}};
            potentialMoves.push_back(captureMove);
        }
    }
    if (currentFile != H) {
        ChessField potentialCaptureTargetField = {currentFile + 1, currentRank + direction};
        auto potentialCaptureTarget = board.getField(potentialCaptureTargetField);
        if (potentialCaptureTarget.has_value() && std::get<ColorIdx>(potentialCaptureTarget.value()) != color) {
            Move captureMove = {cp, currentField, potentialCaptureTargetField, {MoveModifier::CAPTURE}};
            if (std::get<ChessRankIdx>(potentialCaptureTargetField) == 8 || std::get<ChessRankIdx>(potentialCaptureTargetField) == 1) {
                captureMove.addModifier(MoveModifier::CAPTURE);
                captureMove.addModifier(MoveModifier::PROMOTE_BISHOP);
                potentialMoves.push_back(captureMove);
                captureMove.clearModifiers();
                captureMove.addModifier(MoveModifier::CAPTURE);
                captureMove.addModifier(MoveModifier::PROMOTE_KNIGHT);
                potentialMoves.push_back(captureMove);
                captureMove.clearModifiers();
                captureMove.addModifier(MoveModifier::CAPTURE);
                captureMove.addModifier(MoveModifier::PROMOTE_ROOK);
                potentialMoves.push_back(captureMove);
                captureMove.clearModifiers();
                captureMove.addModifier(MoveModifier::CAPTURE);
                captureMove.addModifier(MoveModifier::PROMOTE_QUEEN);
                potentialMoves.push_back(captureMove);
            } else {
                captureMove.addModifier(MoveModifier::CAPTURE);
                potentialMoves.push_back(captureMove);
            }
        } else if (potentialCaptureTargetField == board.getEnPassantTarget()) {
            Move captureMove = {cp, currentField, potentialCaptureTargetField, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT}};
            potentialMoves.push_back(captureMove);
        }
    }

    return potentialMoves;
}

// Checked for hidden recursion: NO
bool addMoveOrIsBlocked(const Board& board, ChessField potentialMoveField, std::vector<Move>& potentialMoves,
                        const ChessPieceOnField& pieceOnField) {
    ChessPiece cp = std::get<ChessPieceIdx>(pieceOnField);
    Color color = std::get<ColorIdx>(cp);
    ChessField currentField = std::get<ChessFieldIdx>(pieceOnField);

    if (std::get<ChessFileIdx>(potentialMoveField) < A || std::get<ChessFileIdx>(potentialMoveField) > H ||
        std::get<ChessRankIdx>(potentialMoveField) < 1 || std::get<ChessRankIdx>(potentialMoveField) > 8)
        return true;

    if (!board.getField(potentialMoveField).has_value()) {
        potentialMoves.push_back(Move{cp, currentField, potentialMoveField});
    } else if (std::get<ColorIdx>(board.getField(potentialMoveField).value()) == color) {
        return true;
    } else {
        potentialMoves.push_back(Move{cp, currentField, potentialMoveField, {MoveModifier::CAPTURE}});
        return true;
    }
    return false;
}

// Checked for hidden recursion: NO
std::vector<Move> ChessRules::getPotentialRookMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    ChessField currentField = std::get<ChessFieldIdx>(pieceOnField);
    ChessFile currentFile = std::get<ChessFileIdx>(currentField);
    ChessRank currentRank = std::get<ChessRankIdx>(currentField);

    for (int file = currentFile + 1; file <= H; ++file) {
        ChessField potentialMoveField = {file, currentRank};
        if (addMoveOrIsBlocked(board, potentialMoveField, potentialMoves, pieceOnField)) break;
    }

    for (int file = currentFile - 1; file >= A; --file) {
        ChessField potentialMoveField = {file, currentRank};
        if (addMoveOrIsBlocked(board, potentialMoveField, potentialMoves, pieceOnField)) break;
    }

    for (int rank = currentRank + 1; rank <= 8; ++rank) {
        ChessField potentialMoveField = {currentFile, rank};
        if (addMoveOrIsBlocked(board, potentialMoveField, potentialMoves, pieceOnField)) break;
    }

    for (int rank = currentRank - 1; rank >= 1; --rank) {
        ChessField potentialMoveField = {currentFile, rank};
        if (addMoveOrIsBlocked(board, potentialMoveField, potentialMoves, pieceOnField)) break;
    }

    return potentialMoves;
}

// Checked for hidden recursion: NO
std::vector<Move> ChessRules::getPotentialKnightMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    ChessField currentField = std::get<ChessFieldIdx>(pieceOnField);
    ChessFile currentFile = std::get<ChessFileIdx>(currentField);
    ChessFile currentRank = std::get<ChessRankIdx>(currentField);

    addMoveOrIsBlocked(board, {currentFile - 2, currentRank - 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile - 2, currentRank + 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile + 2, currentRank - 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile + 2, currentRank + 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile - 1, currentRank - 2}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile - 1, currentRank + 2}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile + 1, currentRank - 2}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile + 1, currentRank + 2}, potentialMoves, pieceOnField);

    return potentialMoves;
}

// Checked for hidden recursion: NO
std::vector<Move> ChessRules::getPotentialBishopMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    ChessField currentField = std::get<ChessFieldIdx>(pieceOnField);
    ChessFile currentFile = std::get<ChessFileIdx>(currentField);
    ChessFile currentRank = std::get<ChessRankIdx>(currentField);

    for (int distance = 1; currentFile + distance <= H && currentRank + distance <= 8; ++distance) {
        ChessField potentialMoveField = {currentFile + distance, currentRank + distance};
        if (addMoveOrIsBlocked(board, potentialMoveField, potentialMoves, pieceOnField)) break;
    }

    for (int distance = 1; currentFile + distance <= H && currentRank - distance >= 1; ++distance) {
        ChessField potentialMoveField = {currentFile + distance, currentRank - distance};
        if (addMoveOrIsBlocked(board, potentialMoveField, potentialMoves, pieceOnField)) break;
    }

    for (int distance = 1; currentFile - distance >= A && currentRank + distance <= 8; ++distance) {
        ChessField potentialMoveField = {currentFile - distance, currentRank + distance};
        if (addMoveOrIsBlocked(board, potentialMoveField, potentialMoves, pieceOnField)) break;
    }

    for (int distance = 1; currentFile - distance >= A && currentRank - distance >= 1; ++distance) {
        ChessField potentialMoveField = {currentFile - distance, currentRank - distance};
        if (addMoveOrIsBlocked(board, potentialMoveField, potentialMoves, pieceOnField)) break;
    }

    return potentialMoves;
}

// Checked for hidden recursion: NO
std::vector<Move> ChessRules::getPotentialQueenMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    base::push_back(potentialMoves, getPotentialRookMoves(board, pieceOnField));
    base::push_back(potentialMoves, getPotentialBishopMoves(board, pieceOnField));

    return potentialMoves;
}

// Checked for hidden recursion: NO
std::vector<Move> ChessRules::getPotentialKingMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    ChessPiece cp = std::get<ChessPieceIdx>(pieceOnField);
    ChessField currentField = std::get<ChessFieldIdx>(pieceOnField);
    ChessFile currentFile = std::get<ChessFileIdx>(currentField);
    ChessFile currentRank = std::get<ChessRankIdx>(currentField);

    addMoveOrIsBlocked(board, {currentFile + 1, currentRank + 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile + 1, currentRank - 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile + 1, currentRank}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile, currentRank + 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile, currentRank - 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile - 1, currentRank + 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile - 1, currentRank - 1}, potentialMoves, pieceOnField);
    addMoveOrIsBlocked(board, {currentFile - 1, currentRank}, potentialMoves, pieceOnField);

    if (board.whosTurnIsIt() == Color::WHITE) {
        if (board.canCastle(Board::Castling::WHITE_LONG)) {
            if (!board.getField({B, 1}).has_value() && !board.getField({C, 1}).has_value() && !board.getField({D, 1}).has_value()) {
                potentialMoves.push_back(Move{cp, currentField, {C, 1}, {MoveModifier::CASTLING_LONG}});
            }
        }
        if (board.canCastle(Board::Castling::WHITE_SHORT)) {
            if (!board.getField({F, 1}).has_value() && !board.getField({G, 1}).has_value()) {
                potentialMoves.push_back(Move{cp, currentField, {G, 1}, {MoveModifier::CASTLING_SHORT}});
            }
        }
    } else {
        if (board.canCastle(Board::Castling::BLACK_LONG)) {
            if (!board.getField({B, 8}).has_value() && !board.getField({C, 8}).has_value() && !board.getField({D, 8}).has_value()) {
                potentialMoves.push_back(Move{cp, currentField, {C, 8}, {MoveModifier::CASTLING_LONG}});
            }
        }
        if (board.canCastle(Board::Castling::BLACK_SHORT)) {
            if (!board.getField({F, 8}).has_value() && !board.getField({G, 8}).has_value()) {
                potentialMoves.push_back(Move{cp, currentField, {G, 8}, {MoveModifier::CASTLING_SHORT}});
            }
        }
    }
    return potentialMoves;
}

bool ChessRules::isCastlingIllegal(const Board& board, const Move& potentialMove) {
    // Todo. Fix / Implement by using      numOfPotentialMovesCoveringField(const Board &board, ChessField field)
    Color movingColor = std::get<ColorIdx>(potentialMove.getChessPiece());
    ChessRank castlingRank = (movingColor == Color::WHITE ? 1 : 8);

    if (potentialMove.hasModifier(MoveModifier::CASTLING_LONG)) {
        if (board.getField({B, castlingRank}).has_value() || board.getField({C, castlingRank}).has_value() ||
            board.getField({D, castlingRank}).has_value()) {
            return true;
        }
        Move move{potentialMove.getChessPiece(), potentialMove.getStartField(), {D, castlingRank}};
        return wouldBeCheck(board, move);

    } else if (potentialMove.hasModifier(MoveModifier::CASTLING_SHORT)) {
        if (board.getField({F, castlingRank}).has_value() || board.getField({G, castlingRank}).has_value()) {
            return true;
        }
        Move move{potentialMove.getChessPiece(), potentialMove.getStartField(), {F, castlingRank}};
        return wouldBeCheck(board, move);
    }

    return false;
}

bool ChessRules::isMoveLegal(const Board& board, const Move& potentialMove, IgnoreCheck ignoreCheck) {
    if (potentialMove.hasModifier(MoveModifier::CASTLING_LONG) || potentialMove.hasModifier(MoveModifier::CASTLING_SHORT)) {
        if (isCastlingIllegal(board, potentialMove)) {
            return false;
        }
    }
    return ignoreCheck == IgnoreCheck::YES || !wouldBeCheck(board, potentialMove);
}

// Todo applyMove in rules. set / unset EnPassant, unset Castling, ...
// Checked for hidden recursion: NO
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

    board.clearField(sf);
    board.setField(ef, cp);
    board.setTurn(movingColor == Color::WHITE ? Color::BLACK : Color::WHITE);
    board.removeEnPassantTarget();
    assert(determineBoardPositionLegality(board) == Legality::LEGAL);
    return true;
}

// IDEA: Return reason for illegal verdict
// Checked for hidden recursion: NO
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
