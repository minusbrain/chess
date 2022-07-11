#include <fmt/core.h>
#include <fmt/format.h>

#include "board_debug.h"
#include "board_factory.h"
#include "move.h"

int main() {
    Board emptyBoard = BoardFactory::createEmptyBoard();
    Board standardBoard = BoardFactory::createStandardBoard();

    fmt::print("Board: {}\n", standardBoard);
    fmt::print("Board: {:b}\n", standardBoard);
    // fmt::print("Board: {:bu}\n", standardBoard);

    Move move1(ChessPiece{Color::WHITE, Piece::ROOK}, A, 1, A, 5);
    Move move2(ChessPiece{Color::WHITE, Piece::KING}, E, 1, G, 1, {MoveModifier::CASTLING_SHORT});

    return 0;
}