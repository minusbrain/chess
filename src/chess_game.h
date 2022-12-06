#pragma once
#include "base/helpers.h"
#include "board.h"
#include "chess_player.h"
// #include "game.h"
#include "move.h"

// using GameStateStorage = Game<Board, Move, void>;

class ChessGame : base::NONCOPYABLE {
   public:
    ChessGame() = delete;
    ChessGame(ChessPlayer& white, ChessPlayer& black);

    void startSyncronousGame(bool fullOutput = true);

   private:
    Board _board;
    ChessPlayer& _white;
    ChessPlayer& _black;
};