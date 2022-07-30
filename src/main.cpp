#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "fmt/core.h"
#include "move.h"
#include "move_debug.h"
#include "rules.h"

int main(int, char**) {
    Board stdBoard = BoardFactory::createStandardBoard();

    fmt::print("Board:\n{:b}\n", stdBoard);

    auto validMovesWhite = ChessRules::getAllValidMoves(stdBoard, Color::WHITE);

    fmt::print("Valid moves for White:\n");
    for (auto validMoveWhite : validMovesWhite) {
        fmt::print("Move: {}\n", validMoveWhite);
    }

    return 0;
}