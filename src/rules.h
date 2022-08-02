#pragma once
#include <vector>

#include "board.h"
#include "move.h"

class ChessRules {
   public:
    static std::vector<Move> getAllValidMoves(const Board& board, bool ignoreCheck = false);
    static std::vector<Move> getAllValidMoves(const Board& board, ChessPieceOnField pieceOnField, bool ignoreCheck);

    static bool isCheck(const Board& board);
    static bool wouldBeCheck(const Board& board, const Move& move);

    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialPawnMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialRookMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialKnightMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialBishopMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialQueenMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialKingMoves(const Board& board, ChessPieceOnField pieceOnField);

    static bool isCastlingIllegal(const Board& board, const Move& potentialMove);
    static bool isMoveLegal(const Board& board, const Move& potentialMove, bool ignoreCheck);
};