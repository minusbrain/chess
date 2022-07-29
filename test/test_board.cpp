#include <gtest/gtest.h>

#include <optional>

#include "../src/board.h"
#include "../src/board_factory.h"
#include "../src/move.h"

TEST(TestChessBoard, A1ShouldBeEmptyOnAnEmptyBoard) {
    Board emptyBoard = BoardFactory::createEmptyBoard();

    ASSERT_EQ(std::nullopt, emptyBoard.getField(A, 1));
}

TEST(TestChessBoard, GetAllPieces_WhiteOnStandardBoard_16Pieces) {
    Board stdBoard = BoardFactory::createStandardBoard();

    auto allWhites = stdBoard.getAllPieces(Color::WHITE);
    ASSERT_EQ(16, allWhites.size());
}

TEST(TestChessBoard, GetAllPieces_BlackOnStandardBoard_16Pieces) {
    Board stdBoard = BoardFactory::createStandardBoard();

    auto allBlacks = stdBoard.getAllPieces(Color::BLACK);
    ASSERT_EQ(16, allBlacks.size());
}

TEST(TestChessBoard, ApplyMove_legalMoveNoCapture_AllowAndProperPostConditions) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 4});

    ASSERT_TRUE(stdBoard.applyMove(movePawn));
    ASSERT_FALSE(stdBoard.getField({A, 2}).has_value());
    ASSERT_TRUE(stdBoard.getField({A, 4}).has_value());
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), stdBoard.getField({A, 4}).value());
}

TEST(TestChessBoard, ApplyMove_MoveOnOccupiedFieldButNoCapture_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 7});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_MoveOnOccupiedFieldCapture_AllowAndProperPostCondition) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 7}, {MoveModifier::CAPTURE});

    ASSERT_TRUE(stdBoard.applyMove(movePawn));
    ASSERT_FALSE(stdBoard.getField({A, 2}).has_value());
    ASSERT_TRUE(stdBoard.getField({A, 7}).has_value());
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), stdBoard.getField({A, 7}).value());
}

TEST(TestChessBoard, ApplyMove_MoveFromEmptyField_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 3}, {A, 6});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_MoveFromFieldOccupiedByWrongPiece_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 1}, {A, 3});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_CaptureFieldWithOwnPiece_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 1}, {MoveModifier::CAPTURE});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_ValidEnPassant_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({B, 5}, {Color::WHITE, Piece::PAWN});
    stdBoard.setField({A, 5}, {Color::BLACK, Piece::PAWN});
    Move movePawn =
        Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_TRUE(stdBoard.applyMove(movePawn));
    ASSERT_FALSE(stdBoard.getField({A, 5}).has_value());
    ASSERT_FALSE(stdBoard.getField({B, 5}).has_value());
    ASSERT_TRUE(stdBoard.getField({A, 6}).has_value());
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), stdBoard.getField({A, 6}).value());
}

TEST(TestChessBoard, ApplyMove_EnPassantButNoCapture_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({B, 5}, {Color::WHITE, Piece::PAWN});
    stdBoard.setField({A, 5}, {Color::BLACK, Piece::PAWN});
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButNoTarget_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({B, 5}, {Color::WHITE, Piece::PAWN});
    Board preBoard(stdBoard);
    Move movePawn =
        Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButOwnTarget_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({B, 5}, {Color::WHITE, Piece::PAWN});
    stdBoard.setField({A, 5}, {Color::WHITE, Piece::PAWN});
    Board preBoard(stdBoard);
    Move movePawn =
        Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButWrongdistance_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({C, 5}, {Color::WHITE, Piece::PAWN});
    stdBoard.setField({A, 5}, {Color::BLACK, Piece::PAWN});
    Board preBoard(stdBoard);
    Move movePawn =
        Move({Color::WHITE, Piece::PAWN}, {C, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButWrongPiece_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({B, 5}, {Color::WHITE, Piece::KNIGHT});
    stdBoard.setField({A, 5}, {Color::BLACK, Piece::PAWN});
    Board preBoard(stdBoard);
    Move movePawn =
        Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestBoardHelper, FieldToIndex_CornerFields_CorrectIndex) {
    ASSERT_EQ(0, BoardHelper::fieldToIndex({A, 1}));
    ASSERT_EQ(7, BoardHelper::fieldToIndex({H, 1}));
    ASSERT_EQ(56, BoardHelper::fieldToIndex({A, 8}));
    ASSERT_EQ(63, BoardHelper::fieldToIndex({H, 8}));
}

TEST(TestBoardHelper, IndexToField_CornerFields_CorrectFields) {
    ASSERT_EQ((ChessField{A, 1}), BoardHelper::indexToField(0));
    ASSERT_EQ((ChessField{H, 1}), BoardHelper::indexToField(7));
    ASSERT_EQ((ChessField{A, 8}), BoardHelper::indexToField(56));
    ASSERT_EQ((ChessField{H, 8}), BoardHelper::indexToField(63));
}