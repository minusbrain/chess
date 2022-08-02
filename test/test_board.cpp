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
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

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
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButOwnTarget_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({B, 5}, {Color::WHITE, Piece::PAWN});
    stdBoard.setField({A, 5}, {Color::WHITE, Piece::PAWN});
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButWrongdistance_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({C, 5}, {Color::WHITE, Piece::PAWN});
    stdBoard.setField({A, 5}, {Color::BLACK, Piece::PAWN});
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {C, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_EnPassantButWrongPiece_DisallowAndNothingChanged) {
    Board stdBoard = BoardFactory::createStandardBoard();
    stdBoard.setField({B, 5}, {Color::WHITE, Piece::KNIGHT});
    stdBoard.setField({A, 5}, {Color::BLACK, Piece::PAWN});
    Board preBoard(stdBoard);
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    ASSERT_FALSE(stdBoard.applyMove(movePawn));
    ASSERT_EQ(preBoard, stdBoard);
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToQueen_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_QUEEN});

    ASSERT_TRUE(fenBoard.applyMove(movePawn));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::QUEEN}), fenBoard.getField({E, 8}).value());
    ASSERT_FALSE(fenBoard.getField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToBishop_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_BISHOP});

    ASSERT_TRUE(fenBoard.applyMove(movePawn));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::BISHOP}), fenBoard.getField({E, 8}).value());
    ASSERT_FALSE(fenBoard.getField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToRook_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_ROOK});

    ASSERT_TRUE(fenBoard.applyMove(movePawn));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), fenBoard.getField({E, 8}).value());
    ASSERT_FALSE(fenBoard.getField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToKnight_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_KNIGHT});

    ASSERT_TRUE(fenBoard.applyMove(movePawn));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::KNIGHT}), fenBoard.getField({E, 8}).value());
    ASSERT_FALSE(fenBoard.getField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToQueen_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CAPTURE});

    ASSERT_TRUE(fenBoard.applyMove(movePawn));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::QUEEN}), fenBoard.getField({D, 8}).value());
    ASSERT_FALSE(fenBoard.getField({E, 7}).has_value());
    ASSERT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToBishop_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_BISHOP, MoveModifier::CAPTURE});

    ASSERT_TRUE(fenBoard.applyMove(movePawn));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::BISHOP}), fenBoard.getField({D, 8}).value());
    ASSERT_FALSE(fenBoard.getField({E, 7}).has_value());
    ASSERT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToRook_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_ROOK, MoveModifier::CAPTURE});

    ASSERT_TRUE(fenBoard.applyMove(movePawn));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), fenBoard.getField({D, 8}).value());
    ASSERT_FALSE(fenBoard.getField({E, 7}).has_value());
    ASSERT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToKnight_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move movePawn = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_KNIGHT, MoveModifier::CAPTURE});

    ASSERT_TRUE(fenBoard.applyMove(movePawn));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::KNIGHT}), fenBoard.getField({D, 8}).value());
    ASSERT_FALSE(fenBoard.getField({E, 7}).has_value());
    ASSERT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_WhiteLongCastling_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move castling = Move({Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG});

    ASSERT_TRUE(fenBoard.applyMove(castling));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::KING}), fenBoard.getField({C, 1}).value());
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), fenBoard.getField({D, 1}).value());
    ASSERT_FALSE(fenBoard.getField({A, 1}).has_value());
    ASSERT_FALSE(fenBoard.getField({E, 1}).has_value());
}

TEST(TestChessBoard, ApplyMove_WhiteShortCastling_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move castling = Move({Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT});

    ASSERT_TRUE(fenBoard.applyMove(castling));
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::KING}), fenBoard.getField({G, 1}).value());
    ASSERT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), fenBoard.getField({F, 1}).value());
    ASSERT_FALSE(fenBoard.getField({H, 1}).has_value());
    ASSERT_FALSE(fenBoard.getField({E, 1}).has_value());
}

TEST(TestChessBoard, ApplyMove_BlackLongCastling_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move castling = Move({Color::BLACK, Piece::KING}, {E, 8}, {C, 8}, {MoveModifier::CASTLING_LONG});

    ASSERT_TRUE(fenBoard.applyMove(castling));
    ASSERT_EQ((ChessPiece{Color::BLACK, Piece::KING}), fenBoard.getField({C, 8}).value());
    ASSERT_EQ((ChessPiece{Color::BLACK, Piece::ROOK}), fenBoard.getField({D, 8}).value());
    ASSERT_FALSE(fenBoard.getField({A, 8}).has_value());
    ASSERT_FALSE(fenBoard.getField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_BlackShortCastling_AllowAndProperPostCondition) {
    Board fenBoard = BoardFactory::createBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move castling = Move({Color::BLACK, Piece::KING}, {E, 8}, {G, 8}, {MoveModifier::CASTLING_SHORT});

    ASSERT_TRUE(fenBoard.applyMove(castling));
    ASSERT_EQ((ChessPiece{Color::BLACK, Piece::KING}), fenBoard.getField({G, 8}).value());
    ASSERT_EQ((ChessPiece{Color::BLACK, Piece::ROOK}), fenBoard.getField({F, 8}).value());
    ASSERT_FALSE(fenBoard.getField({H, 8}).has_value());
    ASSERT_FALSE(fenBoard.getField({E, 8}).has_value());
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