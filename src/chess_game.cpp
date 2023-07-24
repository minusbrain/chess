#include "chess_game.h"

#include <base/improve_containers.h>

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "board_debug.h"
#include "board_factory.h"
#include "fmt/core.h"
#include "move_debug.h"
#include "rules.h"
#include "types.h"

ChessGame::ChessGame(ChessPlayer& white, ChessPlayer& black)
    : _board(BoardFactory::createStandardBoard()), _white(white), _black(black), _progress(_board, {}), _state(State::IDLE) {}


ChessGame::State ChessGame::getState() const { return _state; }

void ChessGame::startSyncronousGame(bool fullOutput) {
    if (_state != State::IDLE) {
        fmt::print("ChessGame state is not IDLE. Start a new game instead.\n");
    }
    _state = State::RUNNING;
    ChessPlayer* currentPlayer = &_white;

    if (fullOutput) fmt::print("ChessGame between {} (white) and {} (black)\n", _white.getName(), _black.getName());
    while (!ChessRules::isGameOver(_board)) {
        currentPlayer = (_board.whosTurnIsIt() == Color::WHITE ? &_white : &_black);

        if (fullOutput) fmt::print("\n{:b}\n{}\n", _board, _board.getFENString(true));
        if (fullOutput) fmt::print("It is {}'s turn\n", currentPlayer->getName());

        std::vector<Move> validMoves = ChessRules::getAllValidMoves(_board, false);
        const Move& move = currentPlayer->getMove(_board, validMoves);
        if (fullOutput) fmt::print("{} plays {}\n", currentPlayer->getName(), move);

        // fmt::print("{} - {}\n", _board.getFENString(), move);
        assert(ChessRules::applyMove(_board, move, true));
        _progress.addMove(move, _board, {});
    }

    _state = State::FINISHED;

    if (fullOutput) {
        int stateCtr = 0;
        for (auto state : _progress) {
            if (state == nullptr) {
                fmt::print("Should NOT have happen!");
                break;
            }
            if (state->moveToNext == nullptr) break;
            if (stateCtr % 10 == 0) fmt::print("\n");
            if (stateCtr % 2 == 0) fmt::print("{}. ", (stateCtr / 2) + 1);
            fmt::print("{} ", *(state->moveToNext));
            ++stateCtr;
        }
        fmt::print("\n");
    }

    fmt::print("{}\n", _board.getFENString(true));

    if (ChessRules::isCheckMate(_board)) {
        ChessPlayer& winner = *currentPlayer;
        ChessPlayer& loser = _board.whosTurnIsIt() == Color::WHITE ? _white : _black;

        fmt::print("{} is check-mate {} won\n", loser.getName(), winner.getName());
    } else if (ChessRules::isStaleMate(_board)) {
        fmt::print("Game ends in a tie due to stale-mate \n");
    } else if (_board.getHalfMoveClock() > 50) {
        fmt::print("Game ends in a tie due to 50 move rule\n");
    }
}

void ChessGame::startAsyncronousGame() {
    if (_state != State::IDLE) {
        fmt::print("ChessGame state is not IDLE. Start a new game instead.\n");
    }
    _state = State::RUNNING;
}

bool ChessGame::doAsyncMove(Color color, Move move) {
    assert(_state == State::RUNNING);
    if (color != _board.whosTurnIsIt()) {
        return false;
    }
    auto validMoves = ChessRules::getAllValidMoves(_board);
    if (base::find(validMoves, move) == validMoves.end()) {
        return false;
    }
    assert(ChessRules::applyMove(_board, move, true));
    _progress.addMove(move, _board, {});
    if (ChessRules::isGameOver(_board)) {
        _state = State::FINISHED;
    }
    return true;
}

const Board& ChessGame::getBoard() const { return _board; }