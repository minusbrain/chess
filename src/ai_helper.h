#pragma once
#include "board.h"

struct PositionRating {
    int white_pieces = 0;
    int black_pieces = 0;
};

static const ChessPiece BLACK_PAWN{Color::BLACK, Piece::PAWN};
static const ChessPiece WHITE_PAWN{Color::WHITE, Piece::PAWN};
static const ChessPiece BLACK_ROOK{Color::BLACK, Piece::ROOK};
static const ChessPiece WHITE_ROOK{Color::WHITE, Piece::ROOK};
static const ChessPiece BLACK_KNIGHT{Color::BLACK, Piece::KNIGHT};
static const ChessPiece WHITE_KNIGHT{Color::WHITE, Piece::KNIGHT};
static const ChessPiece BLACK_BISHOP{Color::BLACK, Piece::BISHOP};
static const ChessPiece WHITE_BISHOP{Color::WHITE, Piece::BISHOP};
static const ChessPiece BLACK_QUEEN{Color::BLACK, Piece::QUEEN};
static const ChessPiece WHITE_QUEEN{Color::WHITE, Piece::QUEEN};
static const ChessPiece BLACK_KING{Color::BLACK, Piece::KING};
static const ChessPiece WHITE_KING{Color::WHITE, Piece::KING};

PositionRating getPositionRating(const Board& board);