#pragma once

#include <set>

#include "types.h"

enum class MoveModifier {
    CHECK,
    CHECK_MATE,
    CAPTURE,
    EN_PASSANT,
    CASTLING_SHORT,
    CASTLING_LONG,
    PROMOTE_QUEEN,
    PROMOTE_ROOK,
    PROMOTE_BISHOP,
    PROMOTE_KNIGHT
};

class Move {
   public:
    Move(ChessPiece piece, ChessField start, ChessField end, std::set<MoveModifier> mods = {});
    Move(ChessPiece piece, ChessFile startLine, ChessRank startRow, ChessFile endLine, ChessRank endRow,
         std::set<MoveModifier> mods = {});

    void addModifier(MoveModifier mod);
    std::set<MoveModifier> getModifiers() const;
    bool hasModifier(MoveModifier mod) const;
    void clearModifiers();

    ChessPiece getChessPiece() const;
    ChessField getStartField() const;
    ChessField getEndField() const;

   private:
    ChessPiece _piece;
    ChessField _startField;
    ChessField _endField;

    std::set<MoveModifier> _mods;
};
