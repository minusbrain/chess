#pragma once
#include <fmt/core.h>
#include <fmt/format.h>

#include <cctype>
#include <iterator>

#include "common_debug.h"
#include "move.h"

inline std::string chessPieceToLAN(ChessPiece cp) {
    return std::get<PieceIdx>(cp) == Piece::PAWN ? "" : std::string(1, getDebugCharForPiece(cp, false));
}

inline std::string chessFileToLAN(ChessFile file) {
    std::string ret;
    switch (file) {
        case A:
            ret.append("a");
            break;
        case B:
            ret.append("b");
            break;
        case C:
            ret.append("c");
            break;
        case D:
            ret.append("d");
            break;
        case E:
            ret.append("e");
            break;
        case F:
            ret.append("f");
            break;
        case G:
            ret.append("g");
            break;
        case H:
            ret.append("h");
            break;
    }
    return ret;
}

inline std::string chessRankToLAN(ChessRank rank) { return std::string(1, 0x30 + rank); }

inline std::string chessFieldToLAN(ChessField cf) {
    std::string ret(chessFileToLAN(std::get<ChessFileIdx>(cf)));
    ret.append(chessRankToLAN(std::get<ChessRankIdx>(cf)));

    return ret;
}

inline std::string optionalCaptureIndicator(Move move) {
    std::string ret;
    if (move.hasModifier(MoveModifier::CAPTURE)) ret.append("x");
    return ret;
};

inline std::string optionalMoveSuffixes(Move move) {
    std::string ret;
    if (move.hasModifier(MoveModifier::PROMOTE_QUEEN))
        ret.append("=Q");
    else if (move.hasModifier(MoveModifier::PROMOTE_ROOK))
        ret.append("=R");
    else if (move.hasModifier(MoveModifier::PROMOTE_BISHOP))
        ret.append("=B");
    else if (move.hasModifier(MoveModifier::PROMOTE_KNIGHT))
        ret.append("=N");

    if (move.hasModifier(MoveModifier::CHECK))
        ret.append("+");
    else if (move.hasModifier(MoveModifier::CHECK_MATE))
        ret.append("#");

    if (move.hasModifier(MoveModifier::EN_PASSANT)) ret.append(" e.p.");

    return ret;
};

/**
 * @brief Allows printing out chess-moves with fmt
 *
 * @tparam Move
 */
template <>
struct fmt::formatter<Move> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Move& move, FormatContext& ctx) {
        if (move.hasModifier(MoveModifier::CASTLING_SHORT)) {
            return format_to(ctx.out(), "0-0");
        } else if (move.hasModifier(MoveModifier::CASTLING_LONG)) {
            return format_to(ctx.out(), "0-0-0");
        } else {
            auto cp = move.getChessPiece();
            auto sf = move.getStartField();
            auto ef = move.getEndField();
            return format_to(ctx.out(), "{}{}{}{}{}", chessPieceToLAN(cp), chessFieldToLAN(sf), optionalCaptureIndicator(move),
                             chessFieldToLAN(ef), optionalMoveSuffixes(move));
        }
    }
};

inline std::ostream& operator<<(std::ostream& os, const Move& move) {
    if (move.hasModifier(MoveModifier::CASTLING_SHORT)) {
        os << "0-0";
    } else if (move.hasModifier(MoveModifier::CASTLING_LONG)) {
        os << "0-0-0";
    } else {
        auto cp = move.getChessPiece();
        auto sf = move.getStartField();
        auto ef = move.getEndField();
        os << chessPieceToLAN(cp) << chessFieldToLAN(sf) << optionalCaptureIndicator(move) << chessFieldToLAN(ef)
           << optionalMoveSuffixes(move);
    }
    return os;
}