#include "move.h"

#include <base/improve_containers.h>

Move::Move(ChessPiece piece, ChessField start, ChessField end, std::set<MoveModifier>&& mods)
    : _piece(piece), _startField(start), _endField(end), _mods(mods) {}
Move::Move(ChessPiece piece, ChessFile startLine, ChessRank startRow, ChessFile endLine, ChessRank endRow, std::set<MoveModifier>&& mods)
    : _piece(piece), _startField{startLine, startRow}, _endField{endLine, endRow}, _mods(mods) {}

void Move::addModifier(MoveModifier mod) { _mods.insert(mod); }
void Move::clearModifiers() { _mods.clear(); }
std::set<MoveModifier> Move::getModifiers() const { return _mods; }
bool Move::hasModifier(MoveModifier mod) const { return base::find(_mods, mod) != _mods.end(); }

ChessPiece Move::getChessPiece() const { return _piece; }
ChessField Move::getStartField() const { return _startField; }
ChessField Move::getEndField() const { return _endField; }
