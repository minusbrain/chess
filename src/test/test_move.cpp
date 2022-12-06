#include <gtest/gtest.h>

#include <sstream>

#include "../move.h"
#include "../move_debug.h"
#include "common.h"
#include "fmt/format.h"

TEST(TestMove, DebugOutput_SimplePawnMove) {
    Move move({Color::WHITE, Piece::PAWN}, {E, 2}, {E, 4});

    std::stringstream ss;
    ss << move;
    std::string fmtstring = fmt::format("{}", move);

    EXPECT_EQ("e2e4", ss.str());
    EXPECT_EQ("e2e4", fmtstring);
}

TEST(TestMove, DebugOutput_PawnPromotion) {
    Move move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_QUEEN});

    std::stringstream ss;
    ss << move;
    std::string fmtstring = fmt::format("{}", move);

    EXPECT_EQ("e7e8=Q", ss.str());
    EXPECT_EQ("e7e8=Q", fmtstring);
}

TEST(TestMove, DebugOutput_PawnPromotion_Check) {
    Move move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CHECK});

    std::stringstream ss;
    ss << move;
    std::string fmtstring = fmt::format("{}", move);

    EXPECT_EQ("e7e8=Q+", ss.str());
    EXPECT_EQ("e7e8=Q+", fmtstring);
}

TEST(TestMove, DebugOutput_PawnPromotion_CaptureCheckMate) {
    Move move({Color::WHITE, Piece::PAWN}, {E, 7}, {E, 8}, {MoveModifier::PROMOTE_QUEEN, MoveModifier::CHECK_MATE, MoveModifier::CAPTURE});

    std::stringstream ss;
    ss << move;
    std::string fmtstring = fmt::format("{}", move);

    EXPECT_EQ("e7xe8=Q#", ss.str());
    EXPECT_EQ("e7xe8=Q#", fmtstring);
}

TEST(TestMove, DebugOutput_CastlingShort_SAN) {
    Move move({Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT});

    std::stringstream ss;
    ss << move;
    std::string fmtstring = fmt::format("{}", move);

    EXPECT_EQ("0-0", ss.str());
    EXPECT_EQ("0-0", fmtstring);
}

TEST(TestMove, DebugOutput_CastlingShort_PGN) {
    Move move({Color::WHITE, Piece::KING}, {E, 1}, {G, 1}, {MoveModifier::CASTLING_SHORT});

    std::stringstream ss;
    ss << pgn_notation << move;
    std::string fmtstring = fmt::format("{:p}", move);

    EXPECT_EQ("O-O", ss.str());
    EXPECT_EQ("O-O", fmtstring);
}

TEST(TestMove, DebugOutput_CastlingLong_SAN) {
    Move move({Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG});

    std::stringstream ss;
    ss << move;
    std::string fmtstring = fmt::format("{}", move);

    EXPECT_EQ("0-0-0", ss.str());
    EXPECT_EQ("0-0-0", fmtstring);
}

TEST(TestMove, DebugOutput_CastlingLong_PGN) {
    Move move({Color::WHITE, Piece::KING}, {E, 1}, {C, 1}, {MoveModifier::CASTLING_LONG});

    std::stringstream ss;
    ss << pgn_notation << move;
    std::string fmtstring = fmt::format("{:p}", move);

    EXPECT_EQ("O-O-O", ss.str());
    EXPECT_EQ("O-O-O", fmtstring);
}