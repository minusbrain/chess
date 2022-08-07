#include <base/argparser.h>
#include <base/improve_containers.h>

#include <iostream>

#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "move.h"
#include "move_debug.h"
#include "rules.h"
#include "types.h"

using base::argparser;

int main(int argc, char** argv) {
    std::string fen;

    argparser parser{"run_chess"};
    parser.add_flag("quiet").short_option('q');

    auto options = parser.parse(argc, argv);

    bool quiet = options.is_flag_set("quiet");

    while (std::getline(std::cin, fen)) {
        Board board = BoardFactory::createBoardFromFEN(fen);
        if (!quiet) std::cout << "FEN String " << fen << board;

        Legality legality = ChessRules::determineBoardPositionLegality(board);
        if (!quiet) std::cout << "Board position is " << (legality == Legality::LEGAL ? "legal" : "illegal") << "\n";

        if (legality == Legality::LEGAL) {
            auto validMoves = ChessRules::getAllValidMoves(board);

            if (!quiet)
                std::cout << "Valid moves for " << (board.whosTurnIsIt() == Color::WHITE ? "white" : "black") << ": " << validMoves << "\n";
        }
    }

    return 0;
}