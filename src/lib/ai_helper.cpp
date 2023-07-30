#include "ai_helper.h"

// Pawn: 1 point (or pawn)
// Knight: 3 points.
// Bishop: 3 points.
// Rook: 5 points.
// Queen: 9 points.

PositionRating getPositionRating(const Board& board) {
    PositionRating rating;
    rating.white_pieces += board.countAllPieces([](ChessPiece cp) { return cp == WHITE_PAWN; });
    rating.white_pieces += 3 * board.countAllPieces([](ChessPiece cp) { return cp == WHITE_KNIGHT || cp == WHITE_BISHOP; });
    rating.white_pieces += 5 * board.countAllPieces([](ChessPiece cp) { return cp == WHITE_ROOK; });
    rating.white_pieces += 9 * board.countAllPieces([](ChessPiece cp) { return cp == WHITE_QUEEN; });

    rating.black_pieces += board.countAllPieces([](ChessPiece cp) { return cp == BLACK_PAWN; });
    rating.black_pieces += 3 * board.countAllPieces([](ChessPiece cp) { return cp == BLACK_KNIGHT || cp == BLACK_BISHOP; });
    rating.black_pieces += 5 * board.countAllPieces([](ChessPiece cp) { return cp == BLACK_ROOK; });
    rating.black_pieces += 9 * board.countAllPieces([](ChessPiece cp) { return cp == BLACK_QUEEN; });
    return rating;
}