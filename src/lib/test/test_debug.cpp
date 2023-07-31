#include <gtest/gtest.h>

#include <sstream>

#include "../move.h"
#include "../move_debug.h"
#include "common.h"
#include "fmt/format.h"
#include "types.h"

TEST(TestDebug, ChessPiece_BlackPawn) {
    ChessPiece piece{Color::BLACK, Piece::PAWN};

    std::string fmtstring = fmt::format("{:u}", piece);

    EXPECT_EQ("black pawn", fmtstring);
}

TEST(TestDebug, ChessPiece_WhiteRook) {
    ChessPiece piece{Color::WHITE, Piece::ROOK};

    std::string fmtstring = fmt::format("{:u}", piece);

    EXPECT_EQ("white rook", fmtstring);
}

TEST(TestDebug, ChessPiece_BlackKnight) {
    ChessPiece piece{Color::BLACK, Piece::KNIGHT};

    std::string fmtstring = fmt::format("{:u}", piece);

    EXPECT_EQ("black knight", fmtstring);
}

TEST(TestDebug, ChessPiece_WhiteBishop) {
    ChessPiece piece{Color::WHITE, Piece::BISHOP};

    std::string fmtstring = fmt::format("{:u}", piece);

    EXPECT_EQ("white bishop", fmtstring);
}

TEST(TestDebug, ChessPiece_BlackQueen) {
    ChessPiece piece{Color::BLACK, Piece::QUEEN};

    std::string fmtstring = fmt::format("{:u}", piece);

    EXPECT_EQ("black queen", fmtstring);
}

TEST(TestDebug, ChessPiece_WhiteKing) {
    ChessPiece piece{Color::WHITE, Piece::KING};

    std::string fmtstring = fmt::format("{:u}", piece);

    EXPECT_EQ("white king", fmtstring);
}

TEST(TestDebug, ChessField_A1) {
    ChessField field{A, 1};

    std::string fmtstring = fmt::format("{:u}", field);

    EXPECT_EQ("a1", fmtstring);
}

TEST(TestDebug, ChessField_B2) {
    ChessField field{B, 2};

    std::string fmtstring = fmt::format("{:u}", field);

    EXPECT_EQ("b2", fmtstring);
}

TEST(TestDebug, ChessField_C3) {
    ChessField field{C, 3};

    std::string fmtstring = fmt::format("{:u}", field);

    EXPECT_EQ("c3", fmtstring);
}

TEST(TestDebug, ChessField_D4) {
    ChessField field{D, 4};

    std::string fmtstring = fmt::format("{:u}", field);

    EXPECT_EQ("d4", fmtstring);
}

TEST(TestDebug, ChessField_E5) {
    ChessField field{E, 5};

    std::string fmtstring = fmt::format("{:u}", field);

    EXPECT_EQ("e5", fmtstring);
}

TEST(TestDebug, ChessField_F6) {
    ChessField field{F, 6};

    std::string fmtstring = fmt::format("{:u}", field);

    EXPECT_EQ("f6", fmtstring);
}

TEST(TestDebug, ChessField_G7) {
    ChessField field{G, 7};

    std::string fmtstring = fmt::format("{:u}", field);

    EXPECT_EQ("g7", fmtstring);
}

TEST(TestDebug, ChessField_H8) {
    ChessField field{H, 8};

    std::string fmtstring = fmt::format("{:u}", field);

    EXPECT_EQ("h8", fmtstring);
}
