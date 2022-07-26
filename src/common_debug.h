#pragma once
#include <cctype>

#include "board.h"

inline char getDebugCharForPiece(ChessPiece p, bool caseBasedOnColor = true) {
    char c;
    switch (std::get<1>(p)) {
        case Piece::PAWN:
            c = 'P';
            break;
        case Piece::ROOK:
            c = 'R';
            break;
        case Piece::KNIGHT:
            c = 'N';
            break;
        case Piece::BISHOP:
            c = 'B';
            break;
        case Piece::QUEEN:
            c = 'Q';
            break;
        case Piece::KING:
            c = 'K';
            break;
    }
    if (caseBasedOnColor && std::get<0>(p) == Color::BLACK)
        return (char)std::tolower(c);
    else
        return c;
}
