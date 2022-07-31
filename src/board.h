#pragma once
#include <base/flag_mask.h>

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

#include "move.h"
#include "types.h"

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
    Board(const Board& orig);

    /**
     * @brief Destroy the chess-board object
     */
    ~Board();

    bool operator==(const Board& other) const = default;

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

    /**
     * @brief Get the All Pieces of one color currently on the board
     *
     * @param color
     * @return Set of pieces currently on the board
     */
    std::vector<ChessPieceOnField> getAllPieces(Color color) const;

    /**
     * @brief Applies the requested move. Only very rudimentary checks are applied
     *
     * Applies the move. Removes the piece from the old field. Sets the piece to the new field
     * Neither field must be out of bounds. startField must actually contain the piece. endField
     * must either be empty or an enemy piece must be there. In case there is an enemy piece, the
     * capture modifier must be set. There are no checks if the move is legal. En passant, castling
     * and promotions are done properly but no rules are being checked if those are legal. Just that
     * the pieces involved in the move are actually present and there end fields are free or an enemy
     * to be captures is there.
     *
     * @param move  The move to execute
     * @return bool - Was the move executed?
     */
    bool applyMove(Move move);

    enum class Castling : char { WHITE_SHORT = 0x01, WHITE_LONG = 0x02, BLACK_SHORT = 0x04, BLACK_LONG = 0x08 };
    bool canCastle(Castling castling) const;
    void setCastlingRaw(uint8_t);
    void setCastling(Castling castling);
    void unsetCastling(Castling castling);

    std::optional<ChessField> getEnPassantTarget() const;
    bool hasEnPassantTarget() const;
    void removeEnPassantTarget();
    void setEnPassantTarget(ChessField);

    Color whosTurnIsIt() const;
    void setTurn(Color);

    std::optional<ChessField> findFirstPiece(ChessPiece cp) const;

   private:
    bool isInBounds(ChessField field);
    std::array<std::optional<ChessPiece>, 64> _board;
    base::flag_mask<Castling> _canCastle;
    std::optional<ChessField> _enpassantTarget;
    Color _turn;
};
