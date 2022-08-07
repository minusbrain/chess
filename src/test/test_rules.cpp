#include <gtest/gtest.h>

#include "../board.h"
#include "../board_factory.h"
#include "../move.h"
#include "../rules.h"
#include "common.h"

TEST(TestChessRules, PawnPositions_PawnOnBaseLine_FindAllPossibleWhiteMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/p7/8/8/8/8/P7/K7 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 2}, {A, 3}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 2}, {A, 4}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnOnBaseLine_FindAllPossibleBlackMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/p7/8/8/8/8/P7/K7 b - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {A, 7}, {A, 6}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {A, 7}, {A, 5}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnOnOtherLine_FindAllPossibleWhiteMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/p7/8/8/P7/8/K7 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 3}, {A, 4}}, moves);
    EXPECT_EQ(1, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnOnOtherLine_FindAllPossibleBlackMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/p7/8/8/P7/8/K7 b - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {A, 6}, {A, 5}}, moves);
    EXPECT_EQ(1, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnBlocked_FindAllPossibleWhiteMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/8/p7/P7/8/8/K7 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_EQ(0, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnBlocked_FindAllPossibleBlackMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/8/p7/P7/8/8/K7 b - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_EQ(0, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanCapture_FindAllPossibleWhiteMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/8/1p6/P7/8/8/K7 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 4}, {A, 5}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {A, 4}, {B, 5}, {MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanCapture_FindAllPossibleBlackMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/8/1p6/P7/8/8/K7 b - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {B, 5}, {B, 4}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {B, 5}, {A, 4}, {MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromote_FindAllPossibleWhiteMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/3P4/8/8/8/8/3p4/K7 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_BISHOP}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_ROOK}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_KNIGHT}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_QUEEN}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromote_FindAllPossibleBlackMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/3P4/8/8/8/8/3p4/K7 b - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_BISHOP}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_ROOK}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_KNIGHT}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_QUEEN}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromoteByCapture_FindAllPossibleWhiteMoves) {
    auto board = debugWrappedGetBoardFromFEN("k1rB4/3P4/8/8/8/8/3p4/K1Rb4 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_BISHOP, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_ROOK, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_KNIGHT, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromoteByCapture_FindAllPossibleBlackMoves) {
    auto board = debugWrappedGetBoardFromFEN("k1rB4/3P4/8/8/8/8/3p4/K1Rb4 b - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {C, 1}, {MoveModifier::PROMOTE_BISHOP, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {C, 1}, {MoveModifier::PROMOTE_ROOK, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {C, 1}, {MoveModifier::PROMOTE_KNIGHT, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {C, 1}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanCaptureEnPassant_FindAllPossibleWhiteMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/8/8/3Pp3/8/8/K7 b - d3");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {E, 4}, {E, 3}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {E, 4}, {D, 3}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT}}, moves);
    EXPECT_EQ(2, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, Castling_WhiteKingPathCoveredByEnemy_ExpectNoCastlingOptions) {
    auto board = debugWrappedGetBoardFromFEN("1k1r1r2/8/8/8/8/8/8/R3K2R w QK -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT}}, moves);
    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG}}, moves);
}

TEST(TestChessRules, Castling_BlackKingPathCoveredByEnemy_ExpectNoCastlingOptions) {
    auto board = debugWrappedGetBoardFromFEN("r3k2r/8/8/8/8/8/8/3RKR2 b qk -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_DOESNOT_CONTAIN(Move{{Color::BLACK, Piece::KING}, {E, 8}, {G, 8}, {MoveModifier::CASTLING_SHORT}}, moves);
    EXPECT_DOESNOT_CONTAIN(Move{{Color::BLACK, Piece::KING}, {E, 8}, {C, 8}, {MoveModifier::CASTLING_LONG}}, moves);
}

TEST(TestChessRules, Castling_WhiteKingEndfieldoveredByEnemy_ExpectNoCastlingOptions) {
    auto board = debugWrappedGetBoardFromFEN("2r1k1r1/8/8/8/8/8/8/R3K2R w KQ -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT}}, moves);
    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG}}, moves);
}

TEST(TestChessRules, Castling_WhiteKingCastlingCoveredByPawns_ExpectNoCastlingOptions) {
    auto board = debugWrappedGetBoardFromFEN("4k3/8/8/8/8/8/3p2p1/R3K2R w KQ -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT}}, moves);
    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG}}, moves);
}
