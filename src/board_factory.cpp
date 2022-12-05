#include "board_factory.h"

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include "board.h"
#include "types.h"

Board BoardFactory::createEmptyBoard() { return Board{}; }

Board BoardFactory::createStandardBoard() {
    Board board{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w QKqk -"};
    board.setLegality(Legality::LEGAL);
    return board;
}

//           8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50
// or        8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8
// currently this assumes a valid FEN string and will fail in case of an invalid one
Board BoardFactory::createBoardFromFEN(std::string fen) { return Board{fen}; }
