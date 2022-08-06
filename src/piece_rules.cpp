#include "piece_rules.h"

#include <base/improve_containers.h>

#include <cassert>
#include <map>

#include "board.h"
#include "move.h"

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

std::vector<Move> PawnRules::getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) {
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

char PawnRules::getDebugChar(Color color) { return color == Color::WHITE ? 'P' : 'p'; }

std::vector<Move> BishopRules::getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) {
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

char BishopRules::getDebugChar(Color color) { return color == Color::WHITE ? 'B' : 'b'; }

std::vector<Move> KnightRules::getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) {
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

char KnightRules::getDebugChar(Color color) { return color == Color::WHITE ? 'N' : 'n'; }

std::vector<Move> RookRules::getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) {
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

char RookRules::getDebugChar(Color color) { return color == Color::WHITE ? 'R' : 'r'; }

std::vector<Move> QueenRules::getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    RookRules rook;
    BishopRules bishop;

    base::push_back(potentialMoves, rook.getPotentialMoves(board, pieceOnField));
    base::push_back(potentialMoves, bishop.getPotentialMoves(board, pieceOnField));

    return potentialMoves;
}

char QueenRules::getDebugChar(Color color) { return color == Color::WHITE ? 'Q' : 'q'; }

std::vector<Move> KingRules::getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) {
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

char KingRules::getDebugChar(Color color) { return color == Color::WHITE ? 'K' : 'k'; }

std::vector<Move> DecoyRules::getPotentialMoves(const Board&, ChessPieceOnField) { return {}; }

char DecoyRules::getDebugChar(Color color) { return color == Color::WHITE ? 'X' : 'x'; }