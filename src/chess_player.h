#pragma once
#include <string>
#include <vector>

#include "board.h"
#include "move.h"

class ChessPlayer {
   public:
    ChessPlayer() = delete;
    ChessPlayer(const std::string& name);

    virtual const std::string& getName() const;
    virtual const Move& getMove(const Board& board, const std::vector<Move>& potentialMoves) = 0;

   private:
    std::string _name;
};

class PickFirstChessPlayer : public ChessPlayer {
   public:
    PickFirstChessPlayer(const std::string& name);

    const Move& getMove(const Board& board, const std::vector<Move>& potentialMoves) override;
};

class PickRandomChessPlayer : public ChessPlayer {
   public:
    PickRandomChessPlayer(const std::string& name);

    const Move& getMove(const Board& board, const std::vector<Move>& potentialMoves) override;
};

class HumanChessPlayer : public ChessPlayer {
   public:
    HumanChessPlayer(const std::string& name);

    const Move& getMove(const Board& board, const std::vector<Move>& potentialMoves) override;
};