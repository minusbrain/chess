#include <fmt/core.h>
#include <fmt/format.h>

#include "board_debug.h"
#include "board_factory.h"
#include "move.h"
#include "move_debug.h"
#include "rules.h"

int main() {
    Board emptyBoard = BoardFactory::createEmptyBoard();
    Board standardBoard = BoardFactory::createStandardBoard();

    fmt::print("Board: {}\n", standardBoard);
    // fmt::print("Board: {:b}\n", standardBoard);

    auto validMoves = ChessRules::getAllValidMoves(standardBoard, Color::WHITE);

    for (auto validMove : validMoves) {
        fmt::print("Valid move: {}\n", validMove);
    }

    return 0;
}