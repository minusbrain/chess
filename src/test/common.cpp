#include "common.h"

#include <base/improve_containers.h>

#include "../board.h"
#include "../board_factory.h"
#include "../move.h"
#include "../rules.h"
#include "main.h"

int getNumberOfMovesForPiece(std::vector<Move> moves, ChessPiece piece) {
    return base::count_if(moves, [&piece](const Move& move) { return move.getChessPiece() == piece; });
}

Board debugWrappedGetBoardFromFEN(std::string fen) {
    Board board = BoardFactory::createBoardFromFEN(fen);
    if (CMD_OPTIONS.is_flag_set("debug")) {
        std::cout << "Using board with FEN " << fen << " :" << board << std::flush;
    }
    return board;
}

Board debugWrappedGetStdBoard() {
    Board board = BoardFactory::createStandardBoard();
    if (CMD_OPTIONS.is_flag_set("debug")) {
        std::cout << "Using standard board " << board << std::flush;
    }
    return board;
}

std::vector<Move> debugWrappedGetAllValidMoves(const Board& board) {
    auto moves = ChessRules::getAllValidMoves(board);
    if (CMD_OPTIONS.is_flag_set("debug")) {
        std::cout << "Found the following " << moves.size() << " valid moves: " << moves << "\n" << std::flush;
    }
    return moves;
}

bool debugWrappedApplyMove(Board& board, const Move& move) {
    bool ret = ChessRules::guardedApplyMove(board, move);
    if (CMD_OPTIONS.is_flag_set("debug")) {
        std::cout << "Applying move " << move << "\nResulting board" << board << std::flush;
    }
    return ret;
}