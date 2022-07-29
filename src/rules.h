#pragma once
#include <vector>

#include "board.h"
#include "move.h"

class ChessRules {
   public:
    static std::vector<Move> getAllValidMoves(Board board, Color color);
    static std::vector<Move> getAllValidMoves(Board board, ChessPieceOnField pieceOnField);
};