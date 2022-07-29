#pragma once
#include <vector>

#include "board.h"
#include "move.h"

class ChessRules {
   public:
    static std::vector<Move> getAllValidMoves(Board board, Color color);
    static std::vector<Move> getAllValidMoves(Board board, ChessPieceOnField pieceOnField);

    static std::vector<Move> getPotentialMoves(Board board, ChessPieceOnField pieceOnField);
    static bool isMoveLegal(Board board, Move potentialMove);
};