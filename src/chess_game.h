#pragma once
#include <optional>

#include "base/helpers.h"
#include "board.h"
#include "chess_player.h"
#include "game.h"
#include "move.h"

struct ExtraStateData {};

using ChessGameProgress = Game<Board, Move, ExtraStateData>;

class ChessGame : base::NONCOPYABLE {
   public:
    ChessGame() = delete;
    ChessGame(ChessPlayer& white, ChessPlayer& black);

    void startSyncronousGame(bool fullOutput = true);

    void startAsyncronousGame();
    bool doAsyncMove(Color color, Move move);
    const Board& getBoard() const;

    ChessPlayer& getWhite();
    ChessPlayer& getBlack();
    ChessPlayer& getMovingPlayer();

    enum class State { IDLE, RUNNING, FINISHED };

    State getState() const;

   private:
    Board _board;
    ChessPlayer& _white;
    ChessPlayer& _black;
    ChessGameProgress _progress;
    State _state;
};