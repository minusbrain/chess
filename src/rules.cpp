#include "rules.h"

#include <cassert>

#include "board.h"
#include "move.h"
#include "types.h"

bool ChessRules::isCheck(const Board&, Color) {
    // Todo: Implement
    return true;
};

std::vector<Move> ChessRules::getAllValidMoves(const Board& board) {
    std::vector<Move> validMoves;

    auto allPiecesOfRequestedColor = board.getAllPieces(board.whosTurnIsIt());

    for (auto pieceOnField : allPiecesOfRequestedColor) {
        auto validMovesOfSinglePiece = ChessRules::getAllValidMoves(board, pieceOnField);
        validMoves.insert(validMoves.end(), validMovesOfSinglePiece.begin(), validMovesOfSinglePiece.end());
    };

    return validMoves;
}

// TODO: Refactor to use polymorphic approach to get movement options per piece
std::vector<Move> ChessRules::getAllValidMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> validMoves;

    std::vector<Move> potentialMoves = ChessRules::getPotentialMoves(board, pieceOnField);

    for (auto potentialMove : potentialMoves) {
        if (ChessRules::isMoveLegal(board, potentialMove)) validMoves.push_back(potentialMove);
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
    // TODO: Implement
    std::vector<Move> potentialMoves;
    (void)board;
    (void)pieceOnField;
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
    std::vector<Move> potentialMovesLikeRook = getPotentialRookMoves(board, pieceOnField);
    std::vector<Move> potentialMovesLikeBishop = getPotentialBishopMoves(board, pieceOnField);

    potentialMovesLikeRook.insert(potentialMovesLikeRook.end(), potentialMovesLikeBishop.begin(), potentialMovesLikeBishop.end());

    return potentialMovesLikeRook;
}

std::vector<Move> ChessRules::getPotentialKingMoves(const Board& board, ChessPieceOnField pieceOnField) {
    // TODO: Implement
    std::vector<Move> potentialMoves;
    (void)board;
    (void)pieceOnField;
    return potentialMoves;
}

bool ChessRules::isMoveLegal(const Board& board, Move potentialMove) {
    // TODO Implement
    (void)board;
    (void)potentialMove;
    return true;
}