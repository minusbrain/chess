#include <fmt/core.h>
#include <fmt/format.h>

#include "board_debug.h"
#include "board_factory.h"

int main() {
    Board emptyBoard = BoardFactory::createEmptyBoard();
    Board standardBoard = BoardFactory::createStandardBoard();

    fmt::print("Board: {}\n", standardBoard);
    fmt::print("Board: {:b}\n", standardBoard);
    // fmt::print("Board: {:bu}\n", standardBoard);

    return 0;
}