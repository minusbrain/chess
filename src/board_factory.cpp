#include "board_factory.h"

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include "board.h"
#include "types.h"

Board BoardFactory::createEmptyBoard() { return Board{}; }

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

    board.setCastlingRaw(0xF);  // All four castling options still available
    board.setLegality(Legality::LEGAL);

    return board;
}

//           8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50
// or        8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8
// currently this assumes a valid FEN string and will fail in case of an invalid one
// Todo: Sanity checks
Board BoardFactory::createBoardFromFEN(std::string fen) { return Board{fen}; }
