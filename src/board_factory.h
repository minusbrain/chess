#pragma once
#include <string>

#include "board.h"

/**
 * @brief chess-board factory
 *
 * Allow construction of chess-boards from various sources or
 * with various intial states
 */
class BoardFactory {
   public:
    /**
     * @brief Creates a completely empty chess-board object
     *
     * @return Created empty chess board
     */
    static Board createEmptyBoard();

    /**
     * @brief Creates a chess-board with standard chess-game setup
     *
     * 16 white pieces on rows 1 and 2, 16 black pieces on rows 7 and 8
     * all in standard chess start positions
     *
     * @return Created chess-board
     */
    static Board createStandardBoard();

    /**
     * @brief Creates a chess-board from FEN String
     *
     * Sets Chess Board from FEN String https://www.chess.com/terms/fen-chess
     *
     * @return Created chess-board
     */
    static Board createBoardFromFEN(std::string fen);
};