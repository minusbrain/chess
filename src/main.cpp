#include <base/improve_containers.h>

#include <iostream>

#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "move.h"
#include "move_debug.h"
#include "rules.h"
#include "types.h"

int main(int, char**) {
    std::string fen;

    while (std::getline(std::cin, fen)) {
        Board board = BoardFactory::createBoardFromFEN(fen);
        std::cout << "FEN String " << fen << board;

        Legality legality = ChessRules::determineBoardPositionLegality(board);
        std::cout << "Board position is " << (legality == Legality::LEGAL ? "legal" : "illegal") << "\n";

        if (legality == Legality::LEGAL) {
            auto validMoves = ChessRules::getAllValidMoves(board);

            std::cout << "Valid moves for " << (board.whosTurnIsIt() == Color::WHITE ? "white" : "black") << ": " << validMoves << "\n";
        }
    }

    return 0;
}