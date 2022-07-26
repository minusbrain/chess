#pragma once

#include <base/improve_containers.h>

#include <set>

#include "board.h"

enum class MoveModifier { CHECK, CHECK_MATE, TAKE, EN_PASSANT, CASTLING_SHORT, CASTLING_LONG };

class Move {
   public:
    Move(ChessPiece piece, ChessField start, ChessField end, std::set<MoveModifier> mods = {})
        : _piece(piece), _startField(start), _endField(end), _mods(mods) {}
    Move(ChessPiece piece, ChessLine startLine, ChessRow startRow, ChessLine endLine, ChessRow endRow,
         std::set<MoveModifier> mods = {})
        : _piece(piece), _startField{startLine, startRow}, _endField{endLine, endRow}, _mods(mods) {}

    void addModifier(MoveModifier mod) { _mods.insert(mod); }
    std::set<MoveModifier> getModifiers() const { return _mods; }
    bool hasModifier(MoveModifier mod) const { return base::find(_mods, mod) != _mods.end(); }

    ChessPiece getChessPiece() const { return _piece; }
    ChessField getStartFiled() const { return _startField; }
    ChessField getEndFiled() const { return _endField; }

   private:
    ChessPiece _piece;
    ChessField _startField;
    ChessField _endField;

    std::set<MoveModifier> _mods;
};
