#pragma once

#include <vector>

#include "types.h"

class Board;
class Move;

class PawnRules {
   public:
    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static char getDebugChar(Color color);
};

class BishopRules {
   public:
    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static char getDebugChar(Color color);
};

class KnightRules {
   public:
    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static char getDebugChar(Color color);
};

class RookRules {
   public:
    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static char getDebugChar(Color color);
};

class QueenRules {
   public:
    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static char getDebugChar(Color color);
};

class KingRules {
   public:
    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static char getDebugChar(Color color);
};

class DecoyRules {
   public:
    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static char getDebugChar(Color color);
};