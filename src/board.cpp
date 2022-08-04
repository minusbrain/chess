#include "board.h"

#include <cassert>
#include <cstdlib>
#include <optional>

#include "move.h"
#include "types.h"

Board::Board(const Board& orig)
    : _board(orig._board), _canCastle(orig._canCastle), _enpassantTarget(orig._enpassantTarget), _turn(orig._turn) {}

Board::Board() : _board(), _canCastle(0x00), _enpassantTarget(), _turn(Color::WHITE) {}

Board::~Board() {}

void Board::setField(ChessFile file, ChessRank rank, ChessPiece chessPiece) {
    _board[BoardHelper::fieldToIndex({file, rank})] = chessPiece;
}

void Board::setField(ChessField field, ChessPiece chessPiece) {
    setField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field), chessPiece);
}

void Board::clearField(ChessFile file, ChessRank rank) { _board[BoardHelper::fieldToIndex({file, rank})].reset(); }

void Board::clearField(ChessField field) { clearField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field)); }

std::optional<ChessPiece> Board::getField(ChessFile file, ChessRank rank) const { return _board[BoardHelper::fieldToIndex({file, rank})]; }

std::optional<ChessPiece> Board::getField(ChessField field) const {
    return getField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field));
}

int BoardHelper::fieldToIndex(ChessField field) {
    assert(std::get<ChessRankIdx>(field) > 0 && std::get<ChessRankIdx>(field) < 9 && std::get<ChessFileIdx>(field) > 0 &&
           std::get<ChessFileIdx>(field) < 9);
    return (std::get<ChessRankIdx>(field) - 1) * 8 + std::get<ChessFileIdx>(field) - 1;
}

ChessField BoardHelper::indexToField(int index) {
    ChessFile file = (index % 8);
    ChessRank rank = index / 8;
    return {file + 1, rank + 1};
}

std::vector<ChessPieceOnField> Board::getAllPieces(Color color) const {
    std::vector<ChessPieceOnField> pieces;
    for (int i = 0; i < 64; ++i) {
        if (_board[i].has_value() && std::get<ColorIdx>(_board[i].value()) == color) {
            pieces.push_back({_board[i].value(), BoardHelper::indexToField(i)});
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
    if (endPiece.has_value() && (!move.hasModifier(MoveModifier::CAPTURE) || std::get<ColorIdx>(endPiece.value()) == movingColor)) {
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
        ChessRank castlingRank = (movingColor == Color::WHITE ? 1 : 8);

        if (move.hasModifier(MoveModifier::CASTLING_LONG)) {
            if (getField({B, castlingRank}).has_value() || getField({C, castlingRank}).has_value() ||
                getField({D, castlingRank}).has_value()) {
                return false;
            }

            if (!getField({A, castlingRank}).has_value() || getField({A, castlingRank}).value() != ChessPiece{movingColor, Piece::ROOK}) {
                return false;
            }

            if (!getField({E, castlingRank}).has_value() || getField({E, castlingRank}).value() != ChessPiece{movingColor, Piece::KING}) {
                return false;
            }
            clearField({A, castlingRank});
            setField({D, castlingRank}, {movingColor, Piece::ROOK});
        } else {
            if (getField({F, castlingRank}).has_value() || getField({G, castlingRank}).has_value()) {
                return false;
            }

            if (!getField({H, castlingRank}).has_value() || getField({H, castlingRank}).value() != ChessPiece{movingColor, Piece::ROOK}) {
                return false;
            }

            if (!getField({E, castlingRank}).has_value() || getField({E, castlingRank}).value() != ChessPiece{movingColor, Piece::KING}) {
                return false;
            }
            clearField({H, castlingRank});
            setField({F, castlingRank}, {movingColor, Piece::ROOK});
        }
    } else if (move.hasModifier(MoveModifier::PROMOTE_QUEEN)) {
        std::get<PieceIdx>(cp) = Piece::QUEEN;
    } else if (move.hasModifier(MoveModifier::PROMOTE_BISHOP)) {
        std::get<PieceIdx>(cp) = Piece::BISHOP;
    } else if (move.hasModifier(MoveModifier::PROMOTE_KNIGHT)) {
        std::get<PieceIdx>(cp) = Piece::KNIGHT;
    } else if (move.hasModifier(MoveModifier::PROMOTE_ROOK)) {
        std::get<PieceIdx>(cp) = Piece::ROOK;
    }

    clearField(sf);
    setField(ef, cp);
    setTurn(movingColor == Color::WHITE ? Color::BLACK : Color::WHITE);
    return true;
}

bool Board::canCastle(Castling castling) const { return _canCastle.check(castling); }
void Board::setCastlingRaw(uint8_t value) { _canCastle.raw_set(value); }
void Board::setCastling(Castling castling) { _canCastle.set(castling); }
void Board::unsetCastling(Castling castling) { _canCastle.unset(castling); }

std::optional<ChessField> Board::getEnPassantTarget() const { return _enpassantTarget; }
bool Board::hasEnPassantTarget() const { return _enpassantTarget.has_value(); };
void Board::removeEnPassantTarget() { _enpassantTarget.reset(); }
void Board::setEnPassantTarget(ChessField field) { _enpassantTarget = field; }

Color Board::whosTurnIsIt() const { return _turn; }
void Board::setTurn(Color color) { _turn = color; }

std::optional<ChessField> Board::findFirstPiece(const std::function<bool(ChessPiece)>& predicate) const {
    for (int i = 0; i < 64; ++i) {
        if (_board[i].has_value() && predicate(_board[i].value())) {
            return BoardHelper::indexToField(i);
        }
    }
    return std::nullopt;
}

int Board::countAllPieces(const std::function<bool(ChessPiece)>& predicate) const {
    (void)predicate;
    return 0;
}