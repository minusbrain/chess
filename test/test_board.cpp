#include <fmt/core.h>
#include <gtest/gtest.h>

#include <optional>

#include "../src/board.h"
#include "../src/board_debug.h"
#include "../src/board_factory.h"
#include "../src/move.h"
#include "../src/move_debug.h"
#include "../src/rules.h"

TEST(TestChessBoard, A1ShouldBeEmptyOnAnEmptyBoard) {
    Board emptyBoard = BoardFactory::createEmptyBoard();

    EXPECT_EQ(std::nullopt, emptyBoard.getField(A, 1));
}

TEST(TestChessBoard, GetAllPieces_WhiteOnStandardBoard_16Pieces) {
    Board stdBoard = BoardFactory::createStandardBoard();

    auto allWhites = stdBoard.getAllPieces(Color::WHITE);
    EXPECT_EQ(16, allWhites.size());
}

TEST(TestChessBoard, GetAllPieces_BlackOnStandardBoard_16Pieces) {
    Board stdBoard = BoardFactory::createStandardBoard();

    auto allBlacks = stdBoard.getAllPieces(Color::BLACK);
    EXPECT_EQ(16, allBlacks.size());
}

TEST(TestChessBoard, ApplyMove_legalMoveNoCapture_AllowAndProperPostConditions) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 4});

    EXPECT_TRUE(ChessRules::applyMove(stdBoard, movePawn));
    EXPECT_FALSE(stdBoard.getField({A, 2}).has_value());
    EXPECT_TRUE(stdBoard.getField({A, 4}).has_value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), stdBoard.getField({A, 4}).value());
}

TEST(TestChessBoard, ApplyMove_MoveOnOccupiedFieldButNoCapture_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 7});

    EXPECT_FALSE(ChessRules::applyMove(stdBoard, movePawn));
    EXPECT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_MoveOnOccupiedFieldCapture_AllowAndProperPostCondition) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 7}, {MoveModifier::CAPTURE});

    EXPECT_TRUE(ChessRules::applyMove(stdBoard, movePawn));
    EXPECT_FALSE(stdBoard.getField({A, 2}).has_value());
    EXPECT_TRUE(stdBoard.getField({A, 7}).has_value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), stdBoard.getField({A, 7}).value());
}

TEST(TestChessBoard, ApplyMove_MoveFromEmptyField_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 3}, {A, 6});

    EXPECT_FALSE(ChessRules::applyMove(stdBoard, movePawn));
    EXPECT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_MoveFromFieldOccupiedByWrongPiece_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 1}, {A, 3});

    EXPECT_FALSE(ChessRules::applyMove(stdBoard, movePawn));
    EXPECT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_CaptureFieldWithOwnPiece_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 1}, {MoveModifier::CAPTURE});

    EXPECT_FALSE(ChessRules::applyMove(stdBoard, movePawn));
    EXPECT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_ValidEnPassant_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("rnbqkbnr/1ppppppp/8/pP6/8/8/P1PPPPPP/RNBQKBNR w KQkq a6");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});
    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_FALSE(fenBoard.getField({A, 5}).has_value());
    EXPECT_FALSE(fenBoard.getField({B, 5}).has_value());
    EXPECT_TRUE(fenBoard.getField({A, 6}).has_value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), fenBoard.getField({A, 6}).value());
}

TEST(TestChessBoard, ApplyMove_EnPassantButNoCapture_DisallowAndNothingChanged) {
    Board fenBoard = BoardFactory::createBoardFromFEN("rnbqkbnr/1ppppppp/8/pP6/8/8/P1PPPPPP/RNBQKBNR w KQkq a6");
    Board preBoard(fenBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::EN_PASSANT});

    EXPECT_FALSE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ(preBoard, fenBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButNoTarget_DisallowAndNothingChanged) {
    Board fenBoard = BoardFactory::createBoardFromFEN("rnbqkbnr/pppppppp/8/1P6/8/8/P1PPPPPP/RNBQKBNR w KQkq a6");

    EXPECT_EQ(Legality::ILLEGAL, ChessRules::determineBoardPositionLegality(fenBoard));
}

TEST(TestChessBoard, ApplyMove_EnPassantButOwnTarget_DisallowAndNothingChanged) {
    Board fenBoard = BoardFactory::createBoardFromFEN("rnbqkbnr/pppppppp/8/PP6/8/8/2PPPPPP/RNBQKBNR b KQkq a6");
    EXPECT_EQ(Legality::ILLEGAL, ChessRules::determineBoardPositionLegality(fenBoard));
}

TEST(TestChessBoard, ApplyMove_EnPassantButWrongdistance_DisallowAndNothingChanged) {
    Board fenBoard = BoardFactory::createBoardFromFEN("rnbqkbnr/1ppppppp/8/p1P5/8/8/2PPPPPP/RNBQKBNR w KQkq a6");
    Board preBoard(fenBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {C, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    EXPECT_FALSE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ(preBoard, fenBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButWrongPiece_DisallowAndNothingChanged) {
    Board fenBoard = BoardFactory::createBoardFromFEN("rnbqkbnr/1ppppppp/8/pN6/8/8/PPPPPPPP/R1BQKBNR b KQkq a6");
    Board preBoard(fenBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    EXPECT_FALSE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ(preBoard, fenBoard);
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToQueen_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_QUEEN});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::QUEEN}), fenBoard.getField({E, 8}).value());
    EXPECT_FALSE(fenBoard.getField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToBishop_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_BISHOP});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::BISHOP}), fenBoard.getField({E, 8}).value());
    EXPECT_FALSE(fenBoard.getField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToRook_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_ROOK});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), fenBoard.getField({E, 8}).value());
    EXPECT_FALSE(fenBoard.getField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToKnight_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_KNIGHT});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::KNIGHT}), fenBoard.getField({E, 8}).value());
    EXPECT_FALSE(fenBoard.getField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToQueen_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CAPTURE});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::QUEEN}), fenBoard.getField({D, 8}).value());
    EXPECT_FALSE(fenBoard.getField({E, 7}).has_value());
    EXPECT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToBishop_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_BISHOP, MoveModifier::CAPTURE});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::BISHOP}), fenBoard.getField({D, 8}).value());
    EXPECT_FALSE(fenBoard.getField({E, 7}).has_value());
    EXPECT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToRook_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_ROOK, MoveModifier::CAPTURE});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), fenBoard.getField({D, 8}).value());
    EXPECT_FALSE(fenBoard.getField({E, 7}).has_value());
    EXPECT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToKnight_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_KNIGHT, MoveModifier::CAPTURE});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, movePawn));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::KNIGHT}), fenBoard.getField({D, 8}).value());
    EXPECT_FALSE(fenBoard.getField({E, 7}).has_value());
    EXPECT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_WhiteLongCastling_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move castling = Move({Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, castling));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::KING}), fenBoard.getField({C, 1}).value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), fenBoard.getField({D, 1}).value());
    EXPECT_FALSE(fenBoard.getField({A, 1}).has_value());
    EXPECT_FALSE(fenBoard.getField({E, 1}).has_value());
}

TEST(TestChessBoard, ApplyMove_WhiteShortCastling_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move castling = Move({Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, castling));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::KING}), fenBoard.getField({G, 1}).value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), fenBoard.getField({F, 1}).value());
    EXPECT_FALSE(fenBoard.getField({H, 1}).has_value());
    EXPECT_FALSE(fenBoard.getField({E, 1}).has_value());
}

TEST(TestChessBoard, ApplyMove_BlackLongCastling_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move castling = Move({Color::BLACK, Piece::KING}, {E, 8}, {C, 8}, {MoveModifier::CASTLING_LONG});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, castling));
    EXPECT_EQ((ChessPiece{Color::BLACK, Piece::KING}), fenBoard.getField({C, 8}).value());
    EXPECT_EQ((ChessPiece{Color::BLACK, Piece::ROOK}), fenBoard.getField({D, 8}).value());
    EXPECT_FALSE(fenBoard.getField({A, 8}).has_value());
    EXPECT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_BlackShortCastling_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move castling = Move({Color::BLACK, Piece::KING}, {E, 8}, {G, 8}, {MoveModifier::CASTLING_SHORT});

    EXPECT_TRUE(ChessRules::applyMove(fenBoard, castling));
    EXPECT_EQ((ChessPiece{Color::BLACK, Piece::KING}), fenBoard.getField({G, 8}).value());
    EXPECT_EQ((ChessPiece{Color::BLACK, Piece::ROOK}), fenBoard.getField({F, 8}).value());
    EXPECT_FALSE(fenBoard.getField({H, 8}).has_value());
    EXPECT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestBoardHelper, FieldToIndex_CornerFields_CorrectIndex) {
    EXPECT_EQ(0, BoardHelper::fieldToIndex({A, 1}));
    EXPECT_EQ(7, BoardHelper::fieldToIndex({H, 1}));
    EXPECT_EQ(56, BoardHelper::fieldToIndex({A, 8}));
    EXPECT_EQ(63, BoardHelper::fieldToIndex({H, 8}));
}

TEST(TestBoardHelper, IndexToField_CornerFields_CorrectFields) {
    EXPECT_EQ((ChessField{A, 1}), BoardHelper::indexToField(0));
    EXPECT_EQ((ChessField{H, 1}), BoardHelper::indexToField(7));
    EXPECT_EQ((ChessField{A, 8}), BoardHelper::indexToField(56));
    EXPECT_EQ((ChessField{H, 8}), BoardHelper::indexToField(63));
}