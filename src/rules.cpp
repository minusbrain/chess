#include "rules.h"

#include <base/improve_containers.h>

#include <cassert>

#include "board.h"
#include "board_factory.h"
#include "move.h"
#include "move_debug.h"
#include "types.h"

bool ChessRules::isCheck(const Board& board) {
    std::vector<Move> validMovesOtherColor;
    std::optional<ChessField> kingFieldOpt = board.findFirstPiece({board.whosTurnIsIt(), Piece::KING});
    assert(kingFieldOpt.has_value());
    ChessField kingField = kingFieldOpt.value();

    Board otherPlayerBoard(board);
    otherPlayerBoard.setTurn(board.whosTurnIsIt() == Color::WHITE ? Color::BLACK : Color::WHITE);

    validMovesOtherColor = ChessRules::getAllValidMoves(otherPlayerBoard);
    int numOfKingCaptureMoves = base::count_if(validMovesOtherColor, [&kingField](const Move& move) {
        return (move.getEndField() == kingField && move.hasModifier(MoveModifier::CAPTURE));
    });

    if (numOfKingCaptureMoves > 0)
        return true;
    else
        return false;
};

bool ChessRules::isCheckMate(const Board& board) {
    // Todo Properly implement. We need some kind of caching for Board -> Moves -> Resulting boards
    if (!isCheck(board)) {
        return false;
    }

    return false;
};

int numOfPotentialMovesCoveringField(const Board& board, ChessField field) {
    std::vector<Move> validMoves = ChessRules::getAllValidMoves(board, true);  // true = do not ignore positions moving yourself into check
    int numOfCaptureMoves = base::count_if(
        validMoves, [&field](const Move& move) { return (move.getEndField() == field && move.hasModifier(MoveModifier::CAPTURE)); });

    return numOfCaptureMoves;
}

bool ChessRules::wouldBeCheck(const Board& board, const Move& move) {
    Color color = board.whosTurnIsIt();

    Board otherPlayerBoard = BoardFactory::createBoardByMoveApplication(board, move);
    std::optional<ChessField> kingFieldOpt = otherPlayerBoard.findFirstPiece({color, Piece::KING});
    assert(kingFieldOpt.has_value());
    ChessField kingField = kingFieldOpt.value();

    int numOfKingCaptureMoves = numOfPotentialMovesCoveringField(otherPlayerBoard, kingField);

    if (numOfKingCaptureMoves > 0)
        return true;
    else
        return false;
};

bool ChessRules::wouldBeCheckMate(const Board& board, const Move& move) {
    // Todo Properly implement. We need some kind of caching for Board -> Moves -> Resulting boards
    if (!wouldBeCheck(board, move)) {
        return false;
    }
    return false;
};

std::vector<Move> ChessRules::getAllValidMoves(const Board& board, bool ignoreCheck) {
    std::vector<Move> validMoves;

    auto allPiecesOfRequestedColor = board.getAllPieces(board.whosTurnIsIt());

    for (auto pieceOnField : allPiecesOfRequestedColor) {
        auto validMovesOfSinglePiece = ChessRules::getAllValidMoves(board, pieceOnField, ignoreCheck);
        base::push_back(validMoves, validMovesOfSinglePiece);
    };

    return validMoves;
}

// TODO: Refactor to use polymorphic approach to get movement options per piece
std::vector<Move> ChessRules::getAllValidMoves(const Board& board, ChessPieceOnField pieceOnField, bool ignoreCheck) {
    std::vector<Move> validMoves;

    std::vector<Move> potentialMoves = ChessRules::getPotentialMoves(board, pieceOnField);

    for (auto potentialMove : potentialMoves) {
        if (ChessRules::isMoveLegal(board, potentialMove, ignoreCheck)) validMoves.push_back(potentialMove);
    }

    return validMoves;
}

// TODO: Refactor to use polymorphic approach to get movement options per piece
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
    }
    return {};
}

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

std::vector<Move> ChessRules::getPotentialQueenMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    base::push_back(potentialMoves, getPotentialRookMoves(board, pieceOnField));
    base::push_back(potentialMoves, getPotentialBishopMoves(board, pieceOnField));

    return potentialMoves;
}

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
        Board inbetweenboard(board);
        inbetweenboard.clearField({E, castlingRank});
        inbetweenboard.setField({F, castlingRank}, potentialMove.getChessPiece());
        Move move{potentialMove.getChessPiece(), potentialMove.getStartField(), {F, castlingRank}};
        return wouldBeCheck(board, move);
    }

    return false;
}

bool ChessRules::isMoveLegal(const Board& board, const Move& potentialMove, bool ignoreCheck) {
    if (potentialMove.hasModifier(MoveModifier::CASTLING_LONG) || potentialMove.hasModifier(MoveModifier::CASTLING_SHORT)) {
        if (isCastlingIllegal(board, potentialMove)) {
            return false;
        }
    }
    return ignoreCheck || !wouldBeCheck(board, potentialMove);
}