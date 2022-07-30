#pragma once
#include <cassert>
#include <cctype>

#include "board.h"
#include "types.h"

inline char getDebugCharForPiece(ChessPiece p, bool caseBasedOnColor = true) {
    char c;
    switch (std::get<PieceIdx>(p)) {
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
    if (caseBasedOnColor && std::get<ColorIdx>(p) == Color::BLACK)
        return (char)std::tolower(c);
    else
        return c;
}

inline ChessPiece getChessPieceFromDebugChar(char c) {
    switch (c) {
        case 'r':
            return ChessPiece{Color::BLACK, Piece::ROOK};
        case 'R':
            return ChessPiece{Color::WHITE, Piece::ROOK};
        case 'n':
            return ChessPiece{Color::BLACK, Piece::KNIGHT};
        case 'N':
            return ChessPiece{Color::WHITE, Piece::KNIGHT};
        case 'b':
            return ChessPiece{Color::BLACK, Piece::BISHOP};
        case 'B':
            return ChessPiece{Color::WHITE, Piece::BISHOP};
        case 'q':
            return ChessPiece{Color::BLACK, Piece::QUEEN};
        case 'Q':
            return ChessPiece{Color::WHITE, Piece::QUEEN};
        case 'k':
            return ChessPiece{Color::BLACK, Piece::KING};
        case 'K':
            return ChessPiece{Color::WHITE, Piece::KING};
        case 'p':
            return ChessPiece{Color::BLACK, Piece::PAWN};
        case 'P':
            return ChessPiece{Color::WHITE, Piece::PAWN};
        default:
            assert(false);
    }
}
