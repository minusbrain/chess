#pragma once
#include <vector>

#include "board.h"
#include "move.h"
#include "types.h"

class ChessRules {
   public:
    static std::vector<Move> getAllPotentialMoves(const Board& board);
    static std::vector<Move> getAllValidMoves(const Board& board, IgnoreCheck ignoreCheck = IgnoreCheck::NO);

    static bool isCheck(const Board& board);
    static bool wouldBeCheck(const Board& board, const Move& move);
    static bool isCheckMate(const Board& board);
    static bool wouldBeCheckMate(const Board& board, const Move& move);

    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialPawnMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialRookMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialKnightMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialBishopMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialQueenMoves(const Board& board, ChessPieceOnField pieceOnField);
    static std::vector<Move> getPotentialKingMoves(const Board& board, ChessPieceOnField pieceOnField);

    static bool isCastlingIllegal(const Board& board, const Move& potentialMove);
    static bool isMoveLegal(const Board& board, const Move& potentialMove, IgnoreCheck ignoreCheck);

    static bool isFieldCoveredByColor(const Board& board, ChessField& field, Color color);

    /**
     * @brief Applies the requested move. Only very rudimentary checks are applied
     *
     * Applies the move. Removes the piece from the old field. Sets the piece to the new field
     * Neither field must be out of bounds. startField must actually contain the piece. endField
     * must either be empty or an enemy piece must be there. In case there is an enemy piece, the
     * capture modifier must be set. There are no checks if the move is legal. En passant, castling
     * and promotions are done properly but no rules are being checked if those are legal. Just that
     * the pieces involved in the move are actually present and there end fields are free or an enemy
     * to be captures is there.
     *
     * @param board  The board to apply the move to
     * @param move  The move to execute
     * @return bool - Was the move executed?
     */
    static bool applyMove(Board& board, const Move& move);

    static Legality determineBoardPositionLegality(Board& board);
};