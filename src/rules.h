#pragma once
#include <vector>

#include "types.h"

class Board;
class Move;

class ChessRules {
   public:
    static std::vector<Move> getAllPotentialMoves(const Board& board);
    static std::vector<Move> getAllValidMoves(const Board& board, bool annotate = true);
    static void annotateMoves(const Board& board, std::vector<Move>& moves);

    static bool isCheck(const Board& board);
    static bool isCheckMate(const Board& board, bool checkHint = false);
    static bool isStaleMate(const Board& board, bool checkHint = true);

    static std::vector<Move> getPotentialMoves(const Board& board, ChessPieceOnField pieceOnField);
    static bool isCastlingLegal(const Board& board, const Move& potentialMove);
    static bool isMoveLegal(const Board& board, const Move& potentialMove);

    static bool wouldMoveSelfIntoCheck(const Board& board, const Move& move);
    static bool isFieldCoveredByColor(const Board& board, const ChessField& field, Color color);

    static bool guardedApplyMove(Board& board, const Move& move);

    static Legality determineBoardPositionLegality(Board& board);

   private:
    /**
     * @brief Applies the requested move. Only very rudimentary checks are applied
     *
     * Applies the move. Removes the piece from the old field. Sets the piece to the new field
     * Neither field must be out of bounds. startField must actually contain the piece. endField
     * must either be empty or an enemy piece must be there. In case there is an enemy piece, the
     * capture modifier must be set. There are no checks if the move is legal. En passant, castling
     * and promotions are done properly but no rules are being checked if those are legal. Just that
     * the pieces involved in the move are actually present and there end fields are free or an enemy
     * to be captures is there. Does no legality checks
     *
     * @param board  The board to apply the move to
     * @param move  The move to execute
     * @return bool - Was the move executed?
     */
    static bool applyMove(Board& board, const Move& move);
};