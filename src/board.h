#pragma once
#include <array>
#include <optional>
#include <set>
#include <tuple>

/**
 * @brief Color of a chess-piece
 */
enum class Color { WHITE, BLACK };

/**
 * @brief Type of a chess-piece
 */
enum class Piece { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

/**
 * @brief A chess-piece with color and type
 */
using ChessPiece = std::tuple<Color, Piece>;
static const int ColorIdx = 0;
static const int PieceIdx = 1;

using ChessFile = int;
static const ChessFile A = 1;
static const ChessFile B = 2;
static const ChessFile C = 3;
static const ChessFile D = 4;
static const ChessFile E = 5;
static const ChessFile F = 6;
static const ChessFile G = 7;
static const ChessFile H = 8;

using ChessRank = int;

/**
 * @brief The coordinates of a field on a chess-board
 */
using ChessField = std::tuple<ChessFile, ChessRank>;
static const int ChessFileIdx = 0;
static const int ChessRankIdx = 1;

/**
 * @brief A chess-piece positioned on a field
 */
using ChessPieceOnField = std::tuple<ChessPiece, ChessField>;
static const int ChessPieceIdx = 0;
static const int ChessFieldIdx = 1;

class BoardHelper {
   public:
    /**
     * @brief Internal helper function that translates a field (rank & file) into an index for the array
     *
     * @param field  Field (File and Rank) to translate
     * @returns     Array index to access for given line and row
     */
    static int fieldToIndex(ChessField field);

    /**
     * @brief Internal helper function that translates an array index to a chess-field (rank & file)
     *
     * @param index   Array index to translate
     * @returns    Field (File and Rank) matching the index
     */
    static ChessField indexToField(int index);
};

/**
 * @brief A chess-board with 8x8 fields
 *
 * An 8x8 chess-board that can be created empty or copied from
 * another board. chess-pieces can be set on it, fields can be
 * cleared and fields can be read back.
 */
class Board {
   public:
    /**
     * @brief Construct a new empty chess-board object
     */
    Board();

    /**
     * @brief Construct a new chess-board object as a copy from an original
     */
    Board(Board &orig);

    /**
     * @brief Destroy the chess-board object
     */
    ~Board();

    /**
     * @brief Set a chess-piece on a field of the board
     *
     * If there already is a chess-piece on the same field it is replaced with the new piece.
     * No rules are being checked. This is a dumb board. Every chess-piece can be set
     * on every field. Function throws assert in case you provide a line or row out of bounds.
     *
     * @param line         Line to set the chess piece on (1-8)
     * @param row          Row to set the chess piece on (1-8)
     * @param chessPiece   The chess-piece to set on the field
     */
    void setField(ChessFile line, ChessRank row, ChessPiece chessPiece);

    /**
     * @brief Set a chess-piece on a field of the board
     *
     * If there already is a chess-piece on the same field it is replaced with the new piece.
     * No rules are being checked. This is a dumb board. Every chess-piece can be set
     * on every field. Function throws assert in case the field is out of bounds.
     *
     * @param field        The field to set the chess-piece to
     * @param chessPiece   The chess-piece to set on the field
     */
    void setField(ChessField field, ChessPiece chessPiece);

    /**
     * @brief Clear a field of the board
     *
     * Empties a field on the board. If the field was already empty before this does nothing.
     * No rules are being checked. This is a dumb board.
     * Function throws assert in case you provide a line or row out of bounds.
     *
     * @param line         Line of the field to clear (1-8)
     * @param row          Row of the field to clear (1-8)
     */
    void clearField(ChessFile line, ChessRank row);

    /**
     * @brief Clear a field of the board
     *
     * Empties a field on the board. If the field was already empty before this does nothing.
     * No rules are being checked. This is a dumb board.
     * Function throws assert in case the field is out of bounds.
     *
     * @param field         The field to clear
     */
    void clearField(ChessField field);

    /**
     * @brief Return the chess-piece that is on a field of the board
     *
     * If there is a chess-piece on the field it is returned as a value in an
     * optional variable. Otherwise an empty optional variable is returned.
     * Function throws assert in case you provide a line or row out of bounds.
     *
     * @param line         Line to get the chess piece from (1-8)
     * @param row          Row to get the chess piece from (1-8)
     *
     * @returns Optional chess-piece that occupies the field
     */
    std::optional<ChessPiece> getField(ChessFile line, ChessRank row) const;

    /**
     * @brief Return the chess-piece that is on a field of the board
     *
     * If there is a chess-piece on the field it is returned as a value in an
     * optional variable. Otherwise an empty optional variable is returned.
     * Function throws assert in case field is out of bounds.
     *
     * @param field        The field to get the chess piece from
     *
     * @returns Optional chess-piece that occupies the field
     */
    std::optional<ChessPiece> getField(ChessField field) const;

    std::set<ChessPieceOnField> getAllPieces(Color color) const;

   private:
    std::array<std::optional<ChessPiece>, 64> _board;
};
