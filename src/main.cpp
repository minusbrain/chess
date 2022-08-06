#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "fmt/core.h"
#include "move.h"
#include "move_debug.h"
#include "rules.h"
#include "types.h"

int main(int argc, char** argv) {
    std::string fen;
    if (argc > 1) {
        fen.assign(argv[1]);
    } else {
        fen = "8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50";
    }

    Board fenBoard = BoardFactory::createBoardFromFEN(fen);

    fmt::print("\nFEN String {}", fen);
    fmt::print("\nFEN Board:\n{:b}", fenBoard);

    Legality legality = ChessRules::determineBoardPositionLegality(fenBoard);

    fmt::print("\nBoard position is {}", legality == Legality::LEGAL ? "legal" : "illegal");

    if (legality == Legality::LEGAL) {
        auto validMoves = ChessRules::getAllValidMoves(fenBoard);

        fmt::print("\nValid moves for {}:", (fenBoard.whosTurnIsIt() == Color::WHITE ? "white" : "black"));
        for (auto validMove : validMoves) {
            fmt::print("\nMove: {}", validMove);
        }
    }
    fmt::print("\n");

    return 0;
}