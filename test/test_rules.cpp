// k7/6Pp/4Pp2/1pP1P3/1Pp1p3/4pP2/2P3pP/K7 w - b3

#include <base/improve_containers.h>
#include <gtest/gtest.h>

#include <deque>
#include <optional>
#include <vector>

#include "../src/board.h"
#include "../src/board_factory.h"
#include "../src/move.h"
#include "../src/move_debug.h"
#include "../src/rules.h"

template <class T, class C>
void EXPECT_CONTAINS(T item, C container) {
    EXPECT_NE(container.end(), std::find(container.begin(), container.end(), item))
        << "Container " << container << " does not contain " << item;
}

int getNumberOfMovesForPiece(std::vector<Move> moves, ChessPiece piece) {
    return base::count_if(moves, [&piece](const Move& move) { return move.getChessPiece() == piece; });
}

TEST(TestChessRules, PawnPositions_PawnOnBaseLine_FindAllPossibleWhiteMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/p7/8/8/8/8/P7/K7 w - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 2}, {A, 3}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 2}, {A, 4}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnOnBaseLine_FindAllPossibleBlackMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/p7/8/8/8/8/P7/K7 b - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {A, 7}, {A, 6}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {A, 7}, {A, 5}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnOnOtherLine_FindAllPossibleWhiteMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/8/p7/8/8/P7/8/K7 w - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 3}, {A, 4}}, moves);
    EXPECT_EQ(1, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnOnOtherLine_FindAllPossibleBlackMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/8/p7/8/8/P7/8/K7 b - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {A, 6}, {A, 5}}, moves);
    EXPECT_EQ(1, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnBlocked_FindAllPossibleWhiteMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/8/8/p7/P7/8/8/K7 w - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_EQ(0, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnBlocked_FindAllPossibleBlackMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/8/8/p7/P7/8/8/K7 b - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_EQ(0, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanCapture_FindAllPossibleWhiteMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/8/8/1p6/P7/8/8/K7 w - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 4}, {A, 5}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 4}, {B, 5}, {MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanCapture_FindAllPossibleBlackMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/8/8/1p6/P7/8/8/K7 b - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {B, 5}, {B, 4}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {B, 5}, {A, 4}, {MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromote_FindAllPossibleWhiteMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/3P4/8/8/8/8/3p4/K7 w - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_BISHOP}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_ROOK}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_KNIGHT}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_QUEEN}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromote_FindAllPossibleBlackMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k7/3P4/8/8/8/8/3p4/K7 b - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_BISHOP}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_ROOK}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_KNIGHT}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_QUEEN}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromoteByCapture_FindAllPossibleWhiteMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k1rB4/3P4/8/8/8/8/3p4/K1Rb4 w - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_BISHOP, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_ROOK, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_KNIGHT, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromoteByCapture_FindAllPossibleBlackMoves) {
    Board board = BoardFactory::creatBoardFromFEN("k1rB4/3P4/8/8/8/8/3p4/K1Rb4 b - -");

    auto moves = ChessRules::getAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {C, 1}, {MoveModifier::PROMOTE_BISHOP, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {C, 1}, {MoveModifier::PROMOTE_ROOK, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {C, 1}, {MoveModifier::PROMOTE_KNIGHT, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {C, 1}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}
