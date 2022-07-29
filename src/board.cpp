#include "board.h"

#include <cassert>

Board::Board(Board &orig) : _board(orig._board) {}

Board::Board() : _board() {}

Board::~Board() {}

void Board::setField(ChessFile line, ChessRank row, ChessPiece chessPiece) {
    _board[fieldToIndex(line, row)] = chessPiece;
}

void Board::setField(ChessField field, ChessPiece chessPiece) {
    setField(std::get<0>(field), std::get<1>(field), chessPiece);
}

void Board::clearField(ChessFile line, ChessRank row) { _board[fieldToIndex(line, row)].reset(); }

void Board::clearField(ChessField field) { clearField(std::get<0>(field), std::get<1>(field)); }

std::optional<ChessPiece> Board::getField(ChessFile line, ChessRank row) const {
    return _board[fieldToIndex(line, row)];
}

std::optional<ChessPiece> Board::getField(ChessField field) const {
    return getField(std::get<0>(field), std::get<1>(field));
}

int Board::fieldToIndex(ChessFile file, ChessRank rank) const {
    assert(rank > 0 && rank < 9 && file > 0 && file < 9);
    return (rank - 1) * 8 + file - 1;
}

ChessField Board::indexToField(int index) const {
    // TODO: Test und verify
    ChessRank rank = index % 8;
    ChessFile file = index - (rank * 8);
    return {file, rank};
}

std::set<ChessPieceOnField> Board::getAllPieces(Color color) const {
    // TODO: Implement
    std::set<ChessPieceOnField> pieces;
    (void)color;
    return pieces;
}