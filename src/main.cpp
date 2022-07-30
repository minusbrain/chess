#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "fmt/core.h"
#include "move.h"
#include "move_debug.h"
#include "rules.h"

int main(int argc, char** argv) {
    std::string fen;
    if (argc > 1) {
        fen.assign(argv[1]);
    }

    Board fenBoard = BoardFactory::creatBoardFromFEN(fen);

    fmt::print("\nFEN String {}", fen);
    fmt::print("FEN Board:\n{:b}\n", fenBoard);

    auto validMovesWhite = ChessRules::getAllValidMoves(fenBoard, Color::WHITE);

    fmt::print("Valid moves for White:\n");
    for (auto validMoveWhite : validMovesWhite) {
        fmt::print("Move: {}\n", validMoveWhite);
    }

    return 0;
}