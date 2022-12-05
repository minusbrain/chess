#include "board.h"

#include <cassert>
#include <cstdlib>
#include <optional>

#include "base/improve_containers.h"
#include "move.h"
#include "types.h"

// Checked for hidden recursion: NO
Board::Board(const Board& orig)
    : _board(orig._board),
      _canCastle(orig._canCastle),
      _enpassantTarget(orig._enpassantTarget),
      _turn(orig._turn),
      _legality(orig._legality) {}

// Checked for hidden recursion: NO
Board::Board() : _board(), _canCastle(0x00), _enpassantTarget(), _turn(Color::WHITE), _legality(Legality::UNDETERMINED) {}

// Checked for hidden recursion: NO
Board::~Board() {}

// Checked for hidden recursion: NO
void Board::setField(ChessFile file, ChessRank rank, ChessPiece chessPiece) {
    _board[BoardHelper::fieldToIndex({file, rank})] = chessPiece;
    _legality = Legality::UNDETERMINED;
}

// Checked for hidden recursion: NO
void Board::setField(ChessField field, ChessPiece chessPiece) {
    setField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field), chessPiece);
}

// Checked for hidden recursion: NO
void Board::clearField(ChessFile file, ChessRank rank) {
    _board[BoardHelper::fieldToIndex({file, rank})].reset();
    _legality = Legality::UNDETERMINED;
}

// Checked for hidden recursion: NO
void Board::clearField(ChessField field) { clearField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field)); }

// Checked for hidden recursion: NO
std::optional<ChessPiece> Board::getPieceOnField(ChessFile file, ChessRank rank) const {
    return _board[BoardHelper::fieldToIndex({file, rank})];
}

// Checked for hidden recursion: NO
std::optional<ChessPiece> Board::getPieceOnField(ChessField field) const {
    return getPieceOnField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field));
}

// Checked for hidden recursion: NO
int BoardHelper::fieldToIndex(ChessField field) {
    assert(std::get<ChessRankIdx>(field) > 0 && std::get<ChessRankIdx>(field) < 9 && std::get<ChessFileIdx>(field) > 0 &&
           std::get<ChessFileIdx>(field) < 9);
    return (std::get<ChessRankIdx>(field) - 1) * 8 + std::get<ChessFileIdx>(field) - 1;
}

// Checked for hidden recursion: NO
ChessField BoardHelper::indexToField(int index) {
    ChessFile file = (index % 8);
    ChessRank rank = index / 8;
    return {file + 1, rank + 1};
}

// Checked for hidden recursion: NO
bool BoardHelper::isInBounds(ChessField field) {
    auto rank = std::get<ChessRankIdx>(field);
    auto file = std::get<ChessFileIdx>(field);
    return (rank >= 1 && rank <= 8 && file >= A && file <= H);
}

// Checked for hidden recursion: NO
std::optional<ChessField> BoardHelper::determineEnPassantCaptureTarget(ChessField start, ChessField end) {
    int fileDistance = std::abs(std::get<ChessFileIdx>(start) - std::get<ChessFileIdx>(end));
    int rankDistance = std::abs(std::get<ChessRankIdx>(start) - std::get<ChessRankIdx>(end));

    if (fileDistance != 1 || rankDistance != 1) {
        return std::nullopt;
    }
    return ChessField{std::get<ChessFileIdx>(end), std::get<ChessRankIdx>(start)};
}

// Checked for hidden recursion: NO
std::vector<ChessPieceOnField> Board::getAllPieces(Color color) const {
    std::vector<ChessPieceOnField> pieces;
    for (int i = 0; i < 64; ++i) {
        if (_board[i].has_value() && std::get<ColorIdx>(_board[i].value()) == color) {
            pieces.push_back({_board[i].value(), BoardHelper::indexToField(i)});
        }
    }
    return pieces;
}

// Checked for hidden recursion: NO
bool Board::canCastle(Castling castling) const { return _canCastle.check(castling); }

// Checked for hidden recursion: NO
void Board::setCastlingRaw(uint8_t value) {
    _canCastle.raw_set(value);
    _legality = Legality::UNDETERMINED;
}

// Checked for hidden recursion: NO
void Board::setCastling(Castling castling) {
    _canCastle.set(castling);
    _legality = Legality::UNDETERMINED;
}

// Checked for hidden recursion: NO
void Board::unsetCastling(Castling castling) {
    _canCastle.unset(castling);
    _legality = Legality::UNDETERMINED;
}

// Checked for hidden recursion: NO
std::optional<ChessField> Board::getEnPassantTarget() const { return _enpassantTarget; }
// Checked for hidden recursion: NO
bool Board::hasEnPassantTarget() const { return _enpassantTarget.has_value(); };
// Checked for hidden recursion: NO
void Board::removeEnPassantTarget() { _enpassantTarget.reset(); }
// Checked for hidden recursion: NO
void Board::setEnPassantTarget(ChessField field) {
    _enpassantTarget = field;
    _legality = Legality::UNDETERMINED;
}
// Checked for hidden recursion: NO
Color Board::whosTurnIsIt() const { return _turn; }
// Checked for hidden recursion: NO
void Board::setTurn(Color color) { _turn = color; }
// Checked for hidden recursion: NO
std::optional<ChessField> Board::findFirstPiece(const std::function<bool(ChessPiece)>& predicate) const {
    for (int i = 0; i < 64; ++i) {
        if (_board[i].has_value() && predicate(_board[i].value())) {
            return BoardHelper::indexToField(i);
        }
    }
    return std::nullopt;
}

// Checked for hidden recursion: NO
int Board::countAllPieces(const std::function<bool(ChessPiece)>& predicate) const {
    return base::count_if(_board, [&predicate](auto& piece) { return piece && predicate(*piece); });
}

// Checked for hidden recursion: NO
void Board::setLegality(Legality legality) { _legality = legality; }
// Checked for hidden recursion: NO
Legality Board::getLegality() const { return _legality; }

bool Board::operator==(const Board& other) const {
    return _board == other._board && _canCastle == other._canCastle && _enpassantTarget == other._enpassantTarget && _turn == other._turn;
}

bool Board::isLegalPosition() {
    if (_legality == Legality::UNDETERMINED) {
        _legality = ChessRules::determineBoardPositionLegality(*this);
    }
    return _legality == Legality::LEGAL;
}