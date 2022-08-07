#pragma once
#include <tuple>

/**
 * @brief Color of a chess-piece
 */
enum class Color { WHITE, BLACK };

/**
 * @brief Type of a chess-piece
 */
enum class Piece { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, DECOY };

enum class Legality { UNDETERMINED, LEGAL, ILLEGAL };

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