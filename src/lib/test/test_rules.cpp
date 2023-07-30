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
    auto board = debugWrappedGetBoardFromFEN("8/k2P4/8/8/8/8/K2p4/8 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_BISHOP}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_ROOK}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_KNIGHT}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {D, 8}, {MoveModifier::PROMOTE_QUEEN}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromote_FindAllPossibleBlackMoves) {
    auto board = debugWrappedGetBoardFromFEN("8/k2P4/8/8/8/8/K2p4/8 b - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_BISHOP}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_ROOK}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_KNIGHT}}, moves);
    EXPECT_CONTAINS(Move{{Color::BLACK, Piece::PAWN}, {D, 2}, {D, 1}, {MoveModifier::PROMOTE_QUEEN}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::BLACK, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromoteByCapture_FindAllPossibleWhiteMoves) {
    auto board = debugWrappedGetBoardFromFEN("2rB3k/3P4/8/8/8/8/3p4/2Rb3K w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_BISHOP, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_ROOK, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_KNIGHT, MoveModifier::CAPTURE}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::PAWN}, {D, 7}, {C, 8}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CAPTURE}}, moves);
    EXPECT_EQ(4, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::PAWN}));
}

TEST(TestChessRules, PawnPositions_PawnCanPromoteByCapture_FindAllPossibleBlackMoves) {
    auto board = debugWrappedGetBoardFromFEN("2rB3k/3P4/8/8/8/8/3p4/2Rb3K b - -");
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

TEST(TestChessRules, KingPositions_CornerPosition_FindAllPossibleMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/8/8/8/8/8/K7 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {A, 1}, {A, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {A, 1}, {B, 1}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {A, 1}, {B, 2}}, moves);
    EXPECT_EQ(3, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::KING}));
}

TEST(TestChessRules, KingPositions_SidePosition_FindAllPossibleMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/8/8/8/8/8/4K3 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 1}, {D, 1}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 1}, {D, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 1}, {E, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 1}, {F, 1}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 1}, {F, 2}}, moves);
    EXPECT_EQ(5, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::KING}));
}

TEST(TestChessRules, KingPositions_CenterPosition_FindAllPossibleMoves) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/8/8/8/4K3/8/8 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {D, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {D, 3}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {D, 4}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {E, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {E, 4}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {F, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {F, 3}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {F, 4}}, moves);
    EXPECT_EQ(8, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::KING}));
}

TEST(TestChessRules, KingPositions_CenterPositionButBlockedByOpponent_FindAllPossibleMoves) {
    auto board = debugWrappedGetBoardFromFEN("8/8/8/4k3/8/4K3/8/8 w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {D, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {D, 3}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {E, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {F, 2}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::KING}, {E, 3}, {F, 3}}, moves);
    EXPECT_EQ(5, getNumberOfMovesForPiece(moves, {Color::WHITE, Piece::KING}));
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

TEST(TestChessRules, Check_SimpleCheck_CorrectStates) {
    auto board = debugWrappedGetBoardFromFEN("4k3/8/8/8/8/8/8/r3K3 w - -");

    EXPECT_TRUE(ChessRules::isCheck(board));
    EXPECT_FALSE(ChessRules::isCheckMate(board));
    EXPECT_FALSE(ChessRules::isStaleMate(board));
}

TEST(TestChessRules, Check_SimpleCheckMate_CorrectStates) {
    auto board = debugWrappedGetBoardFromFEN("4k3/8/8/8/8/8/r7/r3K3 w - -");

    EXPECT_TRUE(ChessRules::isCheck(board));
    EXPECT_TRUE(ChessRules::isCheckMate(board));
    EXPECT_FALSE(ChessRules::isStaleMate(board));
}

TEST(TestChessRules, Check_SimpleStaleMate_CorrectStates) {
    auto board = debugWrappedGetBoardFromFEN("4k1r1/8/8/8/8/8/r7/7K w - -");

    EXPECT_FALSE(ChessRules::isCheck(board));
    EXPECT_FALSE(ChessRules::isCheckMate(board));
    EXPECT_TRUE(ChessRules::isStaleMate(board));
}

TEST(TestChessRules, Check_CheckButCoverMovePreventsMate_CorrectStates) {
    auto board = debugWrappedGetBoardFromFEN("4k3/8/8/8/8/1R6/r7/r6K w - -");

    EXPECT_TRUE(ChessRules::isCheck(board));
    EXPECT_FALSE(ChessRules::isCheckMate(board));
    EXPECT_FALSE(ChessRules::isStaleMate(board));
}

TEST(TestChessRules, MoveAnnotation_RookCanSetCheck_ExpectCorrectMoveAnnotations) {
    auto board = debugWrappedGetBoardFromFEN("4k3/8/8/8/8/8/8/R6K w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::ROOK}, {A, 1}, {A, 8}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::ROOK}, {A, 1}, {A, 8}, {MoveModifier::CHECK}}, moves);
    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::ROOK}, {A, 1}, {E, 1}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::ROOK}, {A, 1}, {E, 1}, {MoveModifier::CHECK}}, moves);
}

TEST(TestChessRules, MoveAnnotation_RookCanSetCheckMate_ExpectCorrectMoveAnnotations) {
    auto board = debugWrappedGetBoardFromFEN("4k3/1R6/8/8/8/8/8/R6K w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::ROOK}, {A, 1}, {A, 8}}, moves);
    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::ROOK}, {A, 1}, {A, 8}, {MoveModifier::CHECK}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::ROOK}, {A, 1}, {A, 8}, {MoveModifier::CHECK_MATE}}, moves);
}

TEST(TestChessRules, MoveAnnotation_RookCanSetStaleMate_ExpectCorrectMoveAnnotations) {
    auto board = debugWrappedGetBoardFromFEN("k7/8/1R6/8/8/8/8/1R5K w - -");
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::ROOK}, {B, 6}, {B, 7}}, moves);
    EXPECT_CONTAINS(Move{{Color::WHITE, Piece::ROOK}, {B, 6}, {B, 7}, {MoveModifier::STALE_MATE}}, moves);
}

TEST(TestChessRules, RandomFen_CausedCoreDump_GithubIssue_1) {
    // https://github.com/minusbrain/chess/issues/1
    auto board = debugWrappedGetBoardFromFEN("4N3/N1N3n1/8/N5K1/7R/PR3Bk1/8/6Q1 w - -");
    EXPECT_NO_FATAL_FAILURE(auto moves = debugWrappedGetAllValidMoves(board));
}

TEST(TestChessRules, ApplyMove_Castling_Regression) {
    auto board = debugWrappedGetBoardFromFEN("3nk3/5p2/1r2pb1p/1PPB3n/3pP2P/1p2P1P1/3RNP1R/B3K3 b k - -");
    EXPECT_FALSE(board.isLegalPosition());
}

TEST(TestChessRules, GetAllValidMoves_CastlingAvailable_ButPiecesInBetween) {
    auto board = debugWrappedGetStdBoard();
    auto moves = debugWrappedGetAllValidMoves(board);

    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT}}, moves);
    EXPECT_DOESNOT_CONTAIN(Move{{Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG}}, moves);
}