#include "board.h"

#include <base/improve_containers.h>
#include <base/strings.h>

#include <cassert>
#include <cstdlib>
#include <optional>

#include "common_debug.h"
#include "move.h"
#include "types.h"

Board::Board() : _board(), _canCastle(0x00), _enpassantTarget(), _turn(Color::WHITE), _legality(Legality::UNDETERMINED) {}

ChessField getChessFieldFromString(std::string str) {
    char filechar = str[0];
    char rankchar = str[1];

    ChessFile file = A;
    switch (filechar) {
        case 'a':
        case 'A':
            file = A;
            break;
        case 'b':
        case 'B':
            file = B;
            break;
        case 'c':
        case 'C':
            file = C;
            break;
        case 'd':
        case 'D':
            file = D;
            break;
        case 'e':
        case 'E':
            file = E;
            break;
        case 'f':
        case 'F':
            file = F;
            break;
        case 'g':
        case 'G':
            file = G;
            break;
        case 'h':
        case 'H':
            file = H;
            break;
    }

    return ChessField{file, rankchar - 0x30};
}

Board::Board(const std::string& fen)
    : _board(), _canCastle(0x00), _enpassantTarget(), _turn(Color::WHITE), _legality(Legality::UNDETERMINED) {
    std::vector<std::string> fields = base::split(fen, ' ', 6);
    std::vector<std::string> ranks = base::split(fields[0], '/', 8);

    int rank = 8;
    for (auto rankStr : ranks) {
        int file = A;

        for (char c : rankStr) {
            if (c >= 0x31 && c <= 0x38) {
                int skip = (c - 0x30);
                file += skip;
                continue;
            }
            ChessPiece cp = getChessPieceFromDebugChar(c);
            setField({file, rank}, cp);
            ++file;
        }
        --rank;
    }

    if (fields.size() > 1) {
        Color turn = (fields[1] == "w" ? Color::WHITE : Color::BLACK);
        setTurn(turn);

        if (fields[2].find('Q') != std::string::npos) setCastling(Board::Castling::WHITE_LONG);
        if (fields[2].find('K') != std::string::npos) setCastling(Board::Castling::WHITE_SHORT);
        if (fields[2].find('q') != std::string::npos) setCastling(Board::Castling::BLACK_LONG);
        if (fields[2].find('k') != std::string::npos) setCastling(Board::Castling::BLACK_SHORT);

        if (fields[3] != "-") {
            setEnPassantTarget(getChessFieldFromString(fields[3]));
        }
    }
}

Board::~Board() {}

void Board::setField(ChessFile file, ChessRank rank, ChessPiece chessPiece) {
    _board[BoardHelper::fieldToIndex({file, rank})] = chessPiece;
    _legality = Legality::UNDETERMINED;
}

void Board::setField(ChessField field, ChessPiece chessPiece) {
    setField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field), chessPiece);
}

void Board::clearField(ChessFile file, ChessRank rank) {
    _board[BoardHelper::fieldToIndex({file, rank})].reset();
    _legality = Legality::UNDETERMINED;
}

void Board::clearField(ChessField field) { clearField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field)); }

std::optional<ChessPiece> Board::getPieceOnField(ChessFile file, ChessRank rank) const {
    return _board[BoardHelper::fieldToIndex({file, rank})];
}

std::optional<ChessPiece> Board::getPieceOnField(ChessField field) const {
    return getPieceOnField(std::get<ChessFileIdx>(field), std::get<ChessRankIdx>(field));
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

bool BoardHelper::isInBounds(ChessField field) {
    auto rank = std::get<ChessRankIdx>(field);
    auto file = std::get<ChessFileIdx>(field);
    return (rank >= 1 && rank <= 8 && file >= A && file <= H);
}

std::optional<ChessField> BoardHelper::determineEnPassantCaptureTarget(ChessField start, ChessField end) {
    int fileDistance = std::abs(std::get<ChessFileIdx>(start) - std::get<ChessFileIdx>(end));
    int rankDistance = std::abs(std::get<ChessRankIdx>(start) - std::get<ChessRankIdx>(end));

    if (fileDistance != 1 || rankDistance != 1) {
        return std::nullopt;
    }
    return ChessField{std::get<ChessFileIdx>(end), std::get<ChessRankIdx>(start)};
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

bool Board::canCastle(Castling castling) const { return _canCastle.check(castling); }

void Board::setCastlingRaw(uint8_t value) {
    _canCastle.raw_set(value);
    _legality = Legality::UNDETERMINED;
}

void Board::setCastling(Castling castling) {
    _canCastle.set(castling);
    _legality = Legality::UNDETERMINED;
}

void Board::unsetCastling(Castling castling) {
    _canCastle.unset(castling);
    _legality = Legality::UNDETERMINED;
}

std::optional<ChessField> Board::getEnPassantTarget() const { return _enpassantTarget; }

bool Board::hasEnPassantTarget() const { return _enpassantTarget.has_value(); };

void Board::removeEnPassantTarget() { _enpassantTarget.reset(); }

void Board::setEnPassantTarget(ChessField field) {
    _enpassantTarget = field;
    _legality = Legality::UNDETERMINED;
}

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
    return base::count_if(_board, [&predicate](auto& piece) { return piece && predicate(*piece); });
}

void Board::setLegality(Legality legality) { _legality = legality; }

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