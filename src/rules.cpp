#include "rules.h"

#include "board.h"

std::vector<Move> ChessRules::getAllValidMoves(Board board, Color color) {
    std::vector<Move> validMoves;

    auto allPiecesOfRequestedColor = board.getAllPieces(color);

    for (auto pieceOnField : allPiecesOfRequestedColor) {
        auto validMovesOfSinglePiece = ChessRules::getAllValidMoves(board, pieceOnField);
        validMoves.insert(validMoves.end(), validMovesOfSinglePiece.begin(), validMovesOfSinglePiece.end());
    };

    return validMoves;
}

// TODO: Refactor to use polymorphic approach to get movement options per piece
std::vector<Move> ChessRules::getAllValidMoves(Board board, ChessPieceOnField pieceOnField) {
    std::vector<Move> validMoves;

    // ChessPiece cp = std::get<ChessPieceIdx>(pieceOnField);
    // Color color = std::get<ColorIdx>(cp);
    // Piece piece = std::get<PieceIdx>(cp);
    // ChessField field = std::get<ChessFieldIdx>(pieceOnField);

    std::vector<Move> potentialMoves = ChessRules::getPotentialMoves(board, pieceOnField);

    for (auto potentialMove : potentialMoves) {
        if (ChessRules::isMoveLegal(board, potentialMove)) validMoves.push_back(potentialMove);
    }

    return validMoves;
}

// TODO: Refactor to use polymorphic approach to get movement options per piece
std::vector<Move> ChessRules::getPotentialMoves(Board board, ChessPieceOnField pieceOnField) {
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

std::vector<Move> ChessRules::getPotentialPawnMoves(Board board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    (void)board;
    (void)pieceOnField;
    return potentialMoves;
}

std::vector<Move> ChessRules::getPotentialRookMoves(Board board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    (void)board;
    (void)pieceOnField;
    return potentialMoves;
}

std::vector<Move> ChessRules::getPotentialKnightMoves(Board board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    (void)board;
    (void)pieceOnField;
    return potentialMoves;
}

std::vector<Move> ChessRules::getPotentialBishopMoves(Board board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    (void)board;
    (void)pieceOnField;
    return potentialMoves;
}

std::vector<Move> ChessRules::getPotentialQueenMoves(Board board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    (void)board;
    (void)pieceOnField;
    return potentialMoves;
}

std::vector<Move> ChessRules::getPotentialKingMoves(Board board, ChessPieceOnField pieceOnField) {
    std::vector<Move> potentialMoves;
    (void)board;
    (void)pieceOnField;
    return potentialMoves;
}

bool ChessRules::isMoveLegal(Board board, Move potentialMove) {
    // TODO Implement
    (void)board;
    (void)potentialMove;
    return true;
}