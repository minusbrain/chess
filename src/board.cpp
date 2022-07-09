#include "board.h"

#include <cassert>

Board::Board(Board &orig) : _board(orig._board) {}

Board::Board() : _board() {}

Board::~Board() {}

void Board::setField(ChessLine line, ChessRow row, ChessPiece chessPiece) {
    _board[lineRowToIndex(line, row)] = chessPiece;
}

void Board::setField(ChessField field, ChessPiece chessPiece) {
    setField(std::get<0>(field), std::get<1>(field), chessPiece);
}

void Board::clearField(ChessLine line, ChessRow row) { _board[lineRowToIndex(line, row)].reset(); }

void Board::clearField(ChessField field) { clearField(std::get<0>(field), std::get<1>(field)); }

std::optional<ChessPiece> Board::getField(ChessLine line, ChessRow row) const {
    return _board[lineRowToIndex(line, row)];
}

std::optional<ChessPiece> Board::getField(ChessField field) const {
    return getField(std::get<0>(field), std::get<1>(field));
}

int Board::lineRowToIndex(ChessLine line, ChessRow row) const {
    assert(row > 0 && row < 9 && line > 0 && line < 9);
    return (row - 1) * 8 + line - 1;
}
