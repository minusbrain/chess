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
        Board fenBoard = BoardFactory::createBoardFromFEN(fen);
        std::cout << "FEN String " << fen << fenBoard;

        Legality legality = ChessRules::determineBoardPositionLegality(fenBoard);
        std::cout << "Board position is " << (legality == Legality::LEGAL ? "legal" : "illegal") << "\n";

        if (legality == Legality::LEGAL) {
            auto validMoves = ChessRules::getAllValidMoves(fenBoard);

            std::cout << "Valid moves for " << (fenBoard.whosTurnIsIt() == Color::WHITE ? "white" : "black") << ": " << validMoves << "\n";
        }
    }

    return 0;
}