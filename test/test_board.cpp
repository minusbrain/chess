#include <gtest/gtest.h>

#include <optional>

#include "../src/board.h"
#include "../src/board_factory.h"

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