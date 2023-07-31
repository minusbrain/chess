#pragma once
#include <fmt/core.h>
#include <fmt/format.h>

#include <sstream>

#include "board.h"
#include "common_debug.h"
#include "types.h"

inline std::string getStringFromPiece(const Piece &piece) {
    switch (piece) {
        case Piece::PAWN:
            return "pawn";
        case Piece::ROOK:
            return "rook";
        case Piece::KNIGHT:
            return "knight";
        case Piece::BISHOP:
            return "bishop";
        case Piece::QUEEN:
            return "queen";
        case Piece::KING:
            return "king";
        case Piece::DECOY:
            return "decoy";
        default:
            return "";
    }
}

inline std::string getStringFromChessField(const ChessField &cf) {
    std::stringstream ss;

    ss << (char)(std::get<ChessFileIdx>(cf) - (char)1 + 'a');
    ss << (char)(std::get<ChessRankIdx>(cf) + '0');

    return ss.str();
}

/**
 * @brief Allows printing out chess pieces with fmt
 *
 * @tparam Piece
 */
template <>
struct fmt::formatter<Piece> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Piece &p, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "{}", getStringFromPiece(p));
    }
};

/**
 * @brief Allows printing out chess colors with fmt
 *
 * @tparam Color
 */
template <>
struct fmt::formatter<Color> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Color &col, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "{}", col == Color::WHITE ? "white" : "black");
    }
};

/**
 * @brief Allows printing out chess-pieces with fmt
 *
 * Supports formating option :u to print out more user friendly output
 * Example:
 * @code {.cpp}
 * fmt::print("Piece: {}\n", ChessPiece{Color::BLACK, Piece::PAWN});   // "p"
 * fmt::print("Piece: {:u}\n", ChessPiece{Color::BLACK, Piece::PAWN}); // "black pawn"
 * @endcode
 *
 * @tparam ChessPiece
 */
template <>
struct fmt::formatter<ChessPiece> {
    enum class OutputType { USER, DEBUG } outputType = OutputType::DEBUG;

    constexpr auto parse(format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        while (it != end && *it != '}') {
            if (*it == 'u') {
                outputType = OutputType::USER;
                ++it;
            }
        }

        // Check if reached the end of the range:
        if (it != end && *it != '}') throw format_error("invalid format");

        // Return an iterator past the end of the parsed range:
        return it;
    }

    template <typename FormatContext>
    auto format(const ChessPiece &p, FormatContext &ctx) {
        if (outputType == OutputType::DEBUG)
            return fmt::format_to(ctx.out(), "{}", getDebugCharForPiece(p));
        else
            return fmt::format_to(ctx.out(), "{} {}", std::get<Color>(p), std::get<Piece>(p));
    }
};

/**
 * @brief Allows printing out chess-field with fmt
 *
 * Supports formating option :u to print out more user friendly output
 * Example:
 * @code {.cpp}
 * fmt::print("Field: {}\n", ChessField{A, 1});   // "11"
 * fmt::print("Field: {:u}\n", ChessField{A, 1}); // "a1"
 * @endcode
 *
 * @tparam ChessField
 */
template <>
struct fmt::formatter<ChessField> {
    enum class OutputType { USER, DEBUG } outputType = OutputType::DEBUG;

    constexpr auto parse(format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        while (it != end && *it != '}') {
            if (*it == 'u') {
                outputType = OutputType::USER;
                ++it;
            }
        }

        // Check if reached the end of the range:
        if (it != end && *it != '}') throw format_error("invalid format");

        // Return an iterator past the end of the parsed range:
        return it;
    }

    template <typename FormatContext>
    auto format(const ChessField &f, FormatContext &ctx) {
        if (outputType == OutputType::DEBUG)
            return fmt::format_to(ctx.out(), "{}{}", std::get<ChessFileIdx>(f), std::get<ChessRankIdx>(f));
        else
            return fmt::format_to(ctx.out(), "{}", getStringFromChessField(f));
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
 * @tparam Board
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
    auto format(const Board &board, FormatContext &ctx) {
        auto it = ctx.out();
        if (!big) {
            it = fmt::format_to(it, "\n abcdefgh\n");
            for (int row = 8; row > 0; --row) {
                it = fmt::format_to(it, "{}", row);
                for (int line = A; line <= H; ++line) {
                    auto cp = board.getPieceOnField(line, row);
                    it = (cp.has_value() ? fmt::format_to(it, "{}", cp.value()) : fmt::format_to(it, " "));
                }
                it = fmt::format_to(it, "{}\n", row);
            }
            it = fmt::format_to(it, " abcdefgh\n");
        } else {
            it = fmt::format_to(it, "\n     a     b     c     d     e     f     g     h\n");
            it = fmt::format_to(it, "  +-----+-----+-----+-----+-----+-----+-----+-----+\n");
            for (int row = 8; row > 0; --row) {
                it = fmt::format_to(it, "  |     |     |     |     |     |     |     |     |\n");
                it = fmt::format_to(it, "{} ", row);
                for (int line = 1; line <= H; ++line) {
                    auto cp = board.getPieceOnField(line, row);
                    it = (cp.has_value() ? fmt::format_to(it, "|  {}  ", cp.value()) : fmt::format_to(it, "|     "));
                }
                it = fmt::format_to(it, "| {}\n  |     |     |     |     |     |     |     |     |\n", row);
                it = fmt::format_to(it, "  +-----+-----+-----+-----+-----+-----+-----+-----+\n");
            }
            it = fmt::format_to(it, "     a     b     c     d     e     f     g     h\n");
        }

        return it;
    }
};
