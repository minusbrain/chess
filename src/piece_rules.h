#pragma once

#include <vector>

#include "types.h"

class Board;
class Move;

class ChessPieceRules {
   public:
    ChessPieceRules(){};
    virtual ~ChessPieceRules(){};

    virtual std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField) = 0;
    virtual char getDebugChar(Color color) = 0;
    char getDebugChar() { return getDebugChar(Color::WHITE); };
};

class PawnRules : public ChessPieceRules {
   public:
    PawnRules(){};
    virtual ~PawnRules(){};

    virtual std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    virtual char getDebugChar(Color color);
};

class BishopRules : public ChessPieceRules {
   public:
    BishopRules(){};
    virtual ~BishopRules(){};

    virtual std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    virtual char getDebugChar(Color color);
};

class KnightRules : public ChessPieceRules {
   public:
    KnightRules(){};
    virtual ~KnightRules(){};

    virtual std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    virtual char getDebugChar(Color color);
};

class RookRules : public ChessPieceRules {
   public:
    RookRules(){};
    virtual ~RookRules(){};

    virtual std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    virtual char getDebugChar(Color color);
};

class QueenRules : public ChessPieceRules {
   public:
    QueenRules(){};
    virtual ~QueenRules(){};

    virtual std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    virtual char getDebugChar(Color color);
};

class KingRules : public ChessPieceRules {
   public:
    KingRules(){};
    virtual ~KingRules(){};

    virtual std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    virtual char getDebugChar(Color color);
};

class DecoyRules : public ChessPieceRules {
   public:
    DecoyRules(){};
    virtual ~DecoyRules(){};

    virtual std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    virtual char getDebugChar(Color color);
};