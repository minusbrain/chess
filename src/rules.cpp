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

std::vector<Move> ChessRules::getPotentialMoves(Board board, ChessPieceOnField pieceOnField) {
    // TODO Implement
    (void)board;
    (void)pieceOnField;
    return {};
}

bool ChessRules::isMoveLegal(Board board, Move potentialMove) {
    // TODO Implement
    (void)board;
    (void)potentialMove;
    return true;
}