#include <gtest/gtest.h>

#include "../board.h"
#include "../board_factory.h"
#include "../move.h"
#include "../rules.h"
#include "common.h"

TEST(TestChessBoard, A1ShouldBeEmptyOnAnEmptyBoard) {
    Board board = BoardFactory::createEmptyBoard();

    EXPECT_EQ(std::nullopt, board.getPieceOnField(A, 1));
}

TEST(TestChessBoard, GetAllPieces_WhiteOnStandardBoard_16Pieces) {
    Board board = debugWrappedGetStdBoard();

    auto allWhites = board.getAllPieces(Color::WHITE);
    EXPECT_EQ(16, allWhites.size());
}

TEST(TestChessBoard, GetAllPieces_BlackOnStandardBoard_16Pieces) {
    Board board = debugWrappedGetStdBoard();

    auto allBlacks = board.getAllPieces(Color::BLACK);
    EXPECT_EQ(16, allBlacks.size());
}

TEST(TestChessBoard, ApplyMove_legalMoveNoCapture_AllowAndProperPostConditions) {
    Board board = debugWrappedGetStdBoard();
    Move move = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 4});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_FALSE(board.getPieceOnField({A, 2}).has_value());
    EXPECT_TRUE(board.getPieceOnField({A, 4}).has_value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), board.getPieceOnField({A, 4}).value());
}

TEST(TestChessBoard, ApplyMove_MoveOnOccupiedFieldButNoCapture_DisallowAndNothingChanged) {
    Board board = debugWrappedGetStdBoard();
    Board preBoard(board);
    Move move = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 7});

    EXPECT_FALSE(debugWrappedApplyMove(board, move));
    EXPECT_EQ(preBoard, board);
}

TEST(TestChessBoard, ApplyMove_MoveOnOccupiedFieldCapture_AllowAndProperPostCondition) {
    Board board = debugWrappedGetStdBoard();
    Move move = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 7}, {MoveModifier::CAPTURE});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_FALSE(board.getPieceOnField({A, 2}).has_value());
    EXPECT_TRUE(board.getPieceOnField({A, 7}).has_value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), board.getPieceOnField({A, 7}).value());
}

TEST(TestChessBoard, ApplyMove_MoveFromEmptyField_DisallowAndNothingChanged) {
    Board board = debugWrappedGetStdBoard();
    Board preBoard(board);
    Move move = Move({Color::WHITE, Piece::PAWN}, {A, 3}, {A, 6});

    EXPECT_FALSE(debugWrappedApplyMove(board, move));
    EXPECT_EQ(preBoard, board);
}

TEST(TestChessBoard, ApplyMove_MoveFromFieldOccupiedByWrongPiece_DisallowAndNothingChanged) {
    Board board = debugWrappedGetStdBoard();
    Board preBoard(board);
    Move move = Move({Color::WHITE, Piece::PAWN}, {A, 1}, {A, 3});

    EXPECT_FALSE(debugWrappedApplyMove(board, move));
    EXPECT_EQ(preBoard, board);
}

TEST(TestChessBoard, ApplyMove_CaptureFieldWithOwnPiece_DisallowAndNothingChanged) {
    Board board = debugWrappedGetStdBoard();
    Board preBoard(board);
    Move move = Move({Color::WHITE, Piece::PAWN}, {A, 2}, {A, 1}, {MoveModifier::CAPTURE});

    EXPECT_FALSE(debugWrappedApplyMove(board, move));
    EXPECT_EQ(preBoard, board);
}

TEST(TestChessBoard, ApplyMove_ValidEnPassant_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("rnbqkbnr/1ppppppp/8/pP6/8/8/P1PPPPPP/RNBQKBNR w KQkq a6");
    Move move = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});
    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_FALSE(board.getPieceOnField({A, 5}).has_value());
    EXPECT_FALSE(board.getPieceOnField({B, 5}).has_value());
    EXPECT_TRUE(board.getPieceOnField({A, 6}).has_value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::PAWN}), board.getPieceOnField({A, 6}).value());
}

TEST(TestChessBoard, ApplyMove_EnPassantButNoCapture_DisallowAndNothingChanged) {
    auto board = debugWrappedGetBoardFromFEN("rnbqkbnr/1ppppppp/8/pP6/8/8/P1PPPPPP/RNBQKBNR w KQkq a6");
    Board preBoard(board);
    Move move = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::EN_PASSANT});

    EXPECT_FALSE(debugWrappedApplyMove(board, move));
    EXPECT_EQ(preBoard, board);
}

TEST(TestChessBoard, ApplyMove_EnPassantButNoTarget_DisallowAndNothingChanged) {
    auto board = debugWrappedGetBoardFromFEN("rnbqkbnr/pppppppp/8/1P6/8/8/P1PPPPPP/RNBQKBNR w KQkq a6");

    EXPECT_FALSE(board.isLegalPosition());
}

TEST(TestChessBoard, ApplyMove_EnPassantButOwnTarget_DisallowAndNothingChanged) {
    auto board = debugWrappedGetBoardFromFEN("rnbqkbnr/pppppppp/8/PP6/8/8/2PPPPPP/RNBQKBNR b KQkq a6");
    EXPECT_FALSE(board.isLegalPosition());
}

TEST(TestChessBoard, ApplyMove_EnPassantButWrongdistance_DisallowAndNothingChanged) {
    auto board = debugWrappedGetBoardFromFEN("rnbqkbnr/1ppppppp/8/p1P5/8/8/2PPPPPP/RNBQKBNR w KQkq a6");
    Board preBoard(board);
    Move move = Move({Color::WHITE, Piece::PAWN}, {C, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    EXPECT_FALSE(debugWrappedApplyMove(board, move));
    EXPECT_EQ(preBoard, board);
}

TEST(TestChessBoard, ApplyMove_EnPassantButWrongPiece_DisallowAndNothingChanged) {
    auto board = debugWrappedGetBoardFromFEN("rnbqkbnr/1ppppppp/8/pN6/8/8/PPPPPPPP/R1BQKBNR b KQkq a6");
    Board preBoard(board);
    Move move = Move({Color::WHITE, Piece::PAWN}, {B, 5}, {A, 6}, {MoveModifier::CAPTURE, MoveModifier::EN_PASSANT});

    EXPECT_FALSE(debugWrappedApplyMove(board, move));
    EXPECT_EQ(preBoard, board);
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToQueen_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move move = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_QUEEN});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::QUEEN}), board.getPieceOnField({E, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToBishop_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move move = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_BISHOP});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::BISHOP}), board.getPieceOnField({E, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToRook_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move move = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_ROOK});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), board.getPieceOnField({E, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnPromotionToKnight_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("k7/4P3/8/8/8/8/8/K7 w - -");
    Move move = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_KNIGHT});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::KNIGHT}), board.getPieceOnField({E, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({E, 7}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToQueen_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move move = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CAPTURE});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::QUEEN}), board.getPieceOnField({D, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({E, 7}).has_value());
    EXPECT_FALSE(board.getPieceOnField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToBishop_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move move = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_BISHOP, MoveModifier::CAPTURE});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::BISHOP}), board.getPieceOnField({D, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({E, 7}).has_value());
    EXPECT_FALSE(board.getPieceOnField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToRook_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move move = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_ROOK, MoveModifier::CAPTURE});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), board.getPieceOnField({D, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({E, 7}).has_value());
    EXPECT_FALSE(board.getPieceOnField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_PawnCapturePromotionToKnight_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("k2n4/4P3/8/8/8/8/8/K7 w - -");
    Move move = Move({Color::WHITE, Piece::PAWN}, {E, 7}, {D, 8}, {MoveModifier::PROMOTE_KNIGHT, MoveModifier::CAPTURE});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::KNIGHT}), board.getPieceOnField({D, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({E, 7}).has_value());
    EXPECT_FALSE(board.getPieceOnField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_WhiteLongCastling_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move move = Move({Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::KING}), board.getPieceOnField({C, 1}).value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), board.getPieceOnField({D, 1}).value());
    EXPECT_FALSE(board.getPieceOnField({A, 1}).has_value());
    EXPECT_FALSE(board.getPieceOnField({E, 1}).has_value());
}

TEST(TestChessBoard, ApplyMove_WhiteShortCastling_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move move = Move({Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::KING}), board.getPieceOnField({G, 1}).value());
    EXPECT_EQ((ChessPiece{Color::WHITE, Piece::ROOK}), board.getPieceOnField({F, 1}).value());
    EXPECT_FALSE(board.getPieceOnField({H, 1}).has_value());
    EXPECT_FALSE(board.getPieceOnField({E, 1}).has_value());
}

TEST(TestChessBoard, ApplyMove_BlackLongCastling_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move move = Move({Color::BLACK, Piece::KING}, {E, 8}, {C, 8}, {MoveModifier::CASTLING_LONG});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::BLACK, Piece::KING}), board.getPieceOnField({C, 8}).value());
    EXPECT_EQ((ChessPiece{Color::BLACK, Piece::ROOK}), board.getPieceOnField({D, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({A, 8}).has_value());
    EXPECT_FALSE(board.getPieceOnField({E, 8}).has_value());
}

TEST(TestChessBoard, ApplyMove_BlackShortCastling_AllowAndProperPostCondition) {
    auto board = debugWrappedGetBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w QqKk -");
    Move move = Move({Color::BLACK, Piece::KING}, {E, 8}, {G, 8}, {MoveModifier::CASTLING_SHORT});

    EXPECT_TRUE(debugWrappedApplyMove(board, move));
    EXPECT_EQ((ChessPiece{Color::BLACK, Piece::KING}), board.getPieceOnField({G, 8}).value());
    EXPECT_EQ((ChessPiece{Color::BLACK, Piece::ROOK}), board.getPieceOnField({F, 8}).value());
    EXPECT_FALSE(board.getPieceOnField({H, 8}).has_value());
    EXPECT_FALSE(board.getPieceOnField({E, 8}).has_value());
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

TEST(TestChessBoard, FenStringLoop_1) {
    std::string expected = "1k2Q1N1/1pB2B1n/8/2P2R2/5q2/K2R1b1B/qQR2Q2/3R4 w - -";
    Board board = debugWrappedGetBoardFromFEN(expected);
    EXPECT_EQ(expected, board.getFENString());
}

TEST(TestChessBoard, FenStringStartBoard) {
    std::string expected = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w QKqk -";
    Board board = debugWrappedGetStdBoard();
    EXPECT_EQ(expected, board.getFENString());
}