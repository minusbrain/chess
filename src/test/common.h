#include <base/improve_containers.h>

#include <algorithm>
#include <vector>

#include "../board_debug.h"
#include "../move_debug.h"
#include "../types.h"

class Move;

template <class T, class C>
void EXPECT_CONTAINS(T item, C container) {
    EXPECT_NE(container.end(), std::find(container.begin(), container.end(), item));
}

template <class T, class C>
void EXPECT_DOESNOT_CONTAIN(T item, C container) {
    EXPECT_EQ(container.end(), std::find(container.begin(), container.end(), item));
}

int getNumberOfMovesForPiece(std::vector<Move> moves, ChessPiece piece);
Board debugWrappedGetBoardFromFEN(std::string fen);
Board debugWrappedGetStdBoard();
std::vector<Move> debugWrappedGetAllValidMoves(const Board& board);
bool debugWrappedApplyMove(Board& board, const Move& move);