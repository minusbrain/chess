#include "common.h"

#include <base/improve_containers.h>

#include "../board.h"
#include "../board_factory.h"
#include "../move.h"
#include "../move_debug.h"
#include "../rules.h"
#include "main.h"

int getNumberOfMovesForPiece(std::vector<Move> moves, ChessPiece piece) {
    return base::count_if(moves, [&piece](const Move& move) { return move.getChessPiece() == piece; });
}

Board debugWrappedGetBoardFromFEN(std::string fen) {
    Board board = BoardFactory::createBoardFromFEN(fen);
    if (CMD_OPTIONS.is_flag_set("debug")) {
        fmt::print("Using board with FEN {} : {}", fen, board);
    }
    return board;
}

Board debugWrappedGetStdBoard() {
    Board board = BoardFactory::createStandardBoard();
    if (CMD_OPTIONS.is_flag_set("debug")) {
        fmt::print("Using standard board {}", board);
    }
    return board;
}

std::vector<Move> debugWrappedGetAllValidMoves(const Board& board) {
    auto moves = ChessRules::getAllValidMoves(board);
    if (CMD_OPTIONS.is_flag_set("debug")) {
        fmt::print("Found the following {} valid moves: {}", moves.size(), moves);
    }
    return moves;
}

bool debugWrappedApplyMove(Board& board, const Move& move) {
    bool ret = ChessRules::applyMove(board, move);
    if (CMD_OPTIONS.is_flag_set("debug")) {
        fmt::print("Applying move {}\nResulting board {}", move, board);
    }
    return ret;
}