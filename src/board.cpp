#include "board.h"

#include <cassert>

Board::Board(Board &orig) : _board(orig._board) {}

Board::Board() : _board() {}

Board::~Board() {}

void Board::setField(ChessFile file, ChessRank rank, ChessPiece chessPiece) {
    _board[BoardHelper::fieldToIndex({file, rank})] = chessPiece;
}

void Board::setField(ChessField field, ChessPiece chessPiece) {
    setField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field), chessPiece);
}

void Board::clearField(ChessFile file, ChessRank rank) { _board[BoardHelper::fieldToIndex({file, rank})].reset(); }

void Board::clearField(ChessField field) { clearField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field)); }

std::optional<ChessPiece> Board::getField(ChessFile file, ChessRank rank) const {
    return _board[BoardHelper::fieldToIndex({file, rank})];
}

std::optional<ChessPiece> Board::getField(ChessField field) const {
    return getField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field));
}

int BoardHelper::fieldToIndex(ChessField field) {
    assert(std::get<ChessRankIdx>(field) > 0 && std::get<ChessRankIdx>(field) < 9 &&
           std::get<ChessFileIdx>(field) > 0 && std::get<ChessFileIdx>(field) < 9);
    return (std::get<ChessRankIdx>(field) - 1) * 8 + std::get<ChessFileIdx>(field) - 1;
}

ChessField BoardHelper::indexToField(int index) {
    ChessFile file = (index % 8);
    ChessRank rank = index / 8;
    return {file + 1, rank + 1};
}

std::set<ChessPieceOnField> Board::getAllPieces(Color color) const {
    std::set<ChessPieceOnField> pieces;
    for (int i = 0; i < 64; ++i) {
        if (_board[i].has_value() && std::get<ColorIdx>(_board[i].value()) == color) {
            pieces.insert({_board[i].value(), BoardHelper::indexToField(i)});
        }
    }
    return pieces;
}