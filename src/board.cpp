#include "board.h"

#include <cassert>
#include <cstdlib>
#include <optional>

#include "move.h"
#include "types.h"

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

bool Board::isInBounds(ChessField field) {
    auto rank = std::get<ChessRankIdx>(field);
    auto file = std::get<ChessFileIdx>(field);
    return (rank >= 1 && rank <= 8 && file >= A && file <= H);
}

std::optional<ChessField> determineEnPassantCaptureTarget(ChessField start, ChessField end) {
    int fileDistance = std::abs(std::get<ChessFileIdx>(start) - std::get<ChessFileIdx>(end));
    int rankDistance = std::abs(std::get<ChessRankIdx>(start) - std::get<ChessRankIdx>(end));

    if (fileDistance != 1 || rankDistance != 1) {
        return std::nullopt;
    }
    return ChessField{std::get<ChessFileIdx>(end), std::get<ChessRankIdx>(start)};
}

bool Board::applyMove(Move move) {
    // TODO:
    // castling
    //  * and promotions are done properly but no rules are being checked if those are legal. Just that
    //  * the pieces involved in the move are actually present and there end fields are free or an enemy
    //  * to be captures is there.
    auto cp = move.getChessPiece();
    Color movingColor = std::get<ColorIdx>(cp);
    auto sf = move.getStartField();
    auto ef = move.getEndField();
    assert(isInBounds(sf));
    assert(isInBounds(ef));

    auto startPiece = getField(sf);
    auto endPiece = getField(ef);

    if (!startPiece.has_value() || startPiece.value() != cp) {
        return false;
    }
    if (endPiece.has_value() &&
        (!move.hasModifier(MoveModifier::CAPTURE) || std::get<ColorIdx>(endPiece.value()) == movingColor)) {
        return false;
    }
    if (move.hasModifier(MoveModifier::EN_PASSANT)) {
        if (std::get<PieceIdx>(cp) != Piece::PAWN || !move.hasModifier(MoveModifier::CAPTURE)) {
            return false;
        }
        auto epFieldOpt = determineEnPassantCaptureTarget(sf, ef);
        if (!epFieldOpt.has_value()) {
            return false;
        }
        auto epTargetOpt = getField(epFieldOpt.value());
        if (!epTargetOpt.has_value()) {
            return false;
        }
        auto epTarget = epTargetOpt.value();
        if (std::get<ColorIdx>(epTarget) == movingColor || std::get<PieceIdx>(epTarget) != Piece::PAWN) {
            return false;
        }
        clearField(epFieldOpt.value());
    } else if (move.hasModifier(MoveModifier::CASTLING_LONG) || move.hasModifier(MoveModifier::CASTLING_SHORT)) {
        return false;
    }
    clearField(sf);
    setField(ef, cp);
    return true;
}