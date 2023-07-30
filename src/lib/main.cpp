#include <base/argparser.h>
#include <base/improve_containers.h>

#include <iostream>

#include "bench.h"
#include "board.h"
#include "board_debug.h"
#include "board_factory.h"
#include "chess_game.h"
#include "chess_player.h"
#include "move.h"
#include "move_debug.h"
#include "rules.h"
#include "types.h"

using base::argparser;

base::BenchmarkStatistics CHESS_BENCH;

void parseFENsFromStdin(bool quiet) {
    std::string fen;

    while (std::getline(std::cin, fen)) {
        Board board = BoardFactory::createBoardFromFEN(fen);
        if (!quiet) fmt::print("FEN String {}\n{}", fen, board);

        bool legalPosition = board.isLegalPosition();

        if (!quiet) fmt::print("Board position is {}\n", (legalPosition ? "legal" : "illegal"));

        if (legalPosition) {
            auto validMoves = ChessRules::getAllValidMoves(board);
            if (!quiet) fmt::print("Valid moves for  {}: {}", (board.whosTurnIsIt() == Color::WHITE ? "white" : "black"), validMoves);
        }
    }

    CHESS_BENCH.printStatistics();
}

int main(int argc, char** argv) {
    argparser parser{"run_chess"};

    parser.add_flag("quiet").short_option('q').description("Quiet, no output");
    parser.add_flag("help").short_option('h').description("Print help");
    parser.add_flag("game").short_option('g').description("Play a game of chess");
    parser.add_option<int>("sim").short_option('s').description("Simulate a number of automatic games").default_value(0);
    parser.add_flag("fen").short_option('f').description("Parse FENs from stdin and print board plus possible moves.");

    auto options = parser.parse(argc, argv);

    if (options.is_flag_set("help")) {
        parser.print_help(std::cout);
        return 0;
    }
    bool quiet = options.is_flag_set("quiet");

    if (options.is_flag_set("fen")) parseFENsFromStdin(quiet);

    if (options.is_flag_set("game")) {
        OneMoveDeepBestPositionChessPlayer whitePlayer{"Andreas"};
        HumanConsolePlayer blackPlayer{"Human"};
        ChessGame game{whitePlayer, blackPlayer};
        game.startSyncronousGame();
    } else if (options.get<int>("sim") > 0) {
        fmt::print("Simulate {} games\n", options.get<int>("sim"));
        PickRandomChessPlayer whitePlayer{"Andreas"};
        PickRandomChessPlayer blackPlayer{"Upasna"};
        for (int i = options.get<int>("sim"); i > 0; --i) {
            ChessGame game{whitePlayer, blackPlayer};
            game.startSyncronousGame();
        }
    }

    return 0;
}
