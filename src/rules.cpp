#include "rules.h"

std::vector<Move> ChessRules::getAllValidMoves(Board board, Color color) {
    std::vector<Move> validMoves;

    auto allPiecesOfRequestedColor = board.getAllPieces(color);

    for (auto pieceOnField : allPiecesOfRequestedColor) {
        auto validMovesOfSinglePiece = ChessRules::getAllValidMoves(board, pieceOnField);
        validMoves.insert(validMoves.end(), validMovesOfSinglePiece.begin(), validMovesOfSinglePiece.end());
    };

    return validMoves;
}

std::vector<Move> ChessRules::getAllValidMoves(Board board, ChessPieceOnField pieceOnField) {
    std::vector<Move> validMoves;

    // Todo Implement
    (void)pieceOnField;
    (void)board;

    return validMoves;
}
