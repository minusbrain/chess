#pragma once
#include <vector>

#include "board.h"
#include "move.h"

class ChessRules {
   public:
    static std::vector<Move> getAllValidMoves(const Board& board);
    static std::vector<Move> getAllValidMoves(const Board& board, ChessPieceOnField pieceOnField);

    static bool isCheck(const Board& board, Color color);

    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialPawnMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialRookMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialKnightMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialBishopMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialQueenMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialKingMoves(const Board& board, ChessPieceOnField pieceOnField);
    static bool isMoveLegal(const Board& board, Move potentialMove);
};