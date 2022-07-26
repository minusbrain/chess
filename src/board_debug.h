#pragma once
#include <fmt/core.h>
#include <fmt/format.h>

#include "board.h"
#include "common_debug.h"

/**
 * @brief Allows printing out chess-pieces with fmt
 *
 * @tparam ChessPiece
 */
template <>
struct fmt::formatter<ChessPiece> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const ChessPiece &p, FormatContext &ctx) {
        return format_to(ctx.out(), "{}", getDebugCharForPiece(p));
    }
};

/**
 * @brief Allows printing out chess-boards with fmt
 *
 * Supports formating option :b to print out a big board
 * Example:
 * @code {.cpp}
 * fmt::print("Board: {}\n", standardBoard);
 * fmt::print("Board: {:b}\n", standardBoard);
 * @endcode
 *
 * @tparam ChessPiece
 */
template <>
struct fmt::formatter<Board> {
    bool big = false;

    constexpr auto parse(format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        while (it != end && *it != '}') {
            if (*it == 'b') {
                big = true;
                ++it;
            }
        }

        // Check if reached the end of the range:
        if (it != end && *it != '}') throw format_error("invalid format");

        // Return an iterator past the end of the parsed range:
        return it;
    }

    template <typename FormatContext>
    auto format(const Board &b, FormatContext &ctx) {
        auto it = ctx.out();
        if (!big) {
            it = format_to(it, "\n abcdefgh\n");
            for (int row = 8; row > 0; --row) {
                it = format_to(it, "{}", row);
                for (int line = A; line <= H; ++line) {
                    auto cp = b.getField(line, row);
                    it = (cp.has_value() ? format_to(it, "{}", cp.value()) : format_to(it, " "));
                }
                it = format_to(it, "{}\n", row);
            }
            it = format_to(it, " abcdefgh\n");
        } else {
            it = format_to(it, "\n     a     b     c     d     e     f     g     h\n");
            it = format_to(it, "  +-----+-----+-----+-----+-----+-----+-----+-----+\n");
            for (int row = 8; row > 0; --row) {
                it = format_to(it, "  |     |     |     |     |     |     |     |     |\n");
                it = format_to(it, "{} ", row);
                for (int line = 1; line <= H; ++line) {
                    auto cp = b.getField(line, row);
                    it = (cp.has_value() ? format_to(it, "|  {}  ", cp.value()) : format_to(it, "|     "));
                }
                it = format_to(it, "| {}\n  |     |     |     |     |     |     |     |     |\n", row);
                it = format_to(it, "  +-----+-----+-----+-----+-----+-----+-----+-----+\n");
            }
            it = format_to(it, "     a     b     c     d     e     f     g     h\n");
        }

        return it;
    }
};