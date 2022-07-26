#include <fmt/core.h>
#include <fmt/format.h>

#include "board_debug.h"
#include "board_factory.h"
#include "move.h"
#include "move_debug.h"

int main() {
    Board emptyBoard = BoardFactory::createEmptyBoard();
    Board standardBoard = BoardFactory::createStandardBoard();

    fmt::print("Board: {}\n", standardBoard);
    // fmt::print("Board: {:b}\n", standardBoard);

    Move move1(ChessPiece{Color::WHITE, Piece::PAWN}, D, 2, D, 4);
    // standardBoard.execute_move(move1);
    fmt::print("Move 1: {}\n", move1);
    fmt::print("Board: {}\n", standardBoard);

    Move move2(ChessPiece{Color::BLACK, Piece::KNIGHT}, G, 8, F, 6, {MoveModifier::CAPTURE});
    // standardBoard.execute_move(move2);
    fmt::print("Move 2: {}\n", move2);
    fmt::print("Board: {}\n", standardBoard);

    return 0;
}