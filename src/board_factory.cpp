#include "board_factory.h"

Board BoardFactory::createEmptyBoard() {
    Board board;
    return board;
}

Board BoardFactory::createStandardBoard() {
    Board board;
    board.setField(A, 1, ChessPiece{Color::WHITE, Piece::ROOK});
    board.setField(B, 1, ChessPiece{Color::WHITE, Piece::KNIGHT});
    board.setField(C, 1, ChessPiece{Color::WHITE, Piece::BISHOP});
    board.setField(D, 1, ChessPiece{Color::WHITE, Piece::QUEEN});
    board.setField(E, 1, ChessPiece{Color::WHITE, Piece::KING});
    board.setField(F, 1, ChessPiece{Color::WHITE, Piece::BISHOP});
    board.setField(G, 1, ChessPiece{Color::WHITE, Piece::KNIGHT});
    board.setField(H, 1, ChessPiece{Color::WHITE, Piece::ROOK});
    board.setField(A, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(B, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(C, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(D, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(E, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(F, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(G, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(H, 2, ChessPiece{Color::WHITE, Piece::PAWN});

    board.setField(A, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(B, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(C, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(D, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(E, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(F, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(G, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(H, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(A, 8, ChessPiece{Color::BLACK, Piece::ROOK});
    board.setField(B, 8, ChessPiece{Color::BLACK, Piece::KNIGHT});
    board.setField(C, 8, ChessPiece{Color::BLACK, Piece::BISHOP});
    board.setField(D, 8, ChessPiece{Color::BLACK, Piece::QUEEN});
    board.setField(E, 8, ChessPiece{Color::BLACK, Piece::KING});
    board.setField(F, 8, ChessPiece{Color::BLACK, Piece::BISHOP});
    board.setField(G, 8, ChessPiece{Color::BLACK, Piece::KNIGHT});
    board.setField(H, 8, ChessPiece{Color::BLACK, Piece::ROOK});

    return board;
}