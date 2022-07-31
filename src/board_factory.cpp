#include "board_factory.h"

#include <base/split.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include "common_debug.h"
#include "types.h"

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

Board BoardFactory::createEmptyBoard() {
    Board board;
    return board;
}

Board BoardFactory::createStandardBoard() {
    Board board;
    board.setField(A, 1, ChessPiece{Color::WHITE, Piece::ROOK});
    board.setField(B, 1, ChessPiece{Color::WHITE, Piece::KNIGHT});
    board.setField(C, 1, ChessPiece{Color::WHITE, Piece::BISHOP});
    board.setField(D, 1, ChessPiece{Color::WHITE, Piece::QUEEN});
    board.setField(E, 1, ChessPiece{Color::WHITE, Piece::KING});
    board.setField(F, 1, ChessPiece{Color::WHITE, Piece::BISHOP});
    board.setField(G, 1, ChessPiece{Color::WHITE, Piece::KNIGHT});
    board.setField(H, 1, ChessPiece{Color::WHITE, Piece::ROOK});
    board.setField(A, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(B, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(C, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(D, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(E, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(F, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(G, 2, ChessPiece{Color::WHITE, Piece::PAWN});
    board.setField(H, 2, ChessPiece{Color::WHITE, Piece::PAWN});

    board.setField(A, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(B, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(C, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(D, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(E, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(F, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(G, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(H, 7, ChessPiece{Color::BLACK, Piece::PAWN});
    board.setField(A, 8, ChessPiece{Color::BLACK, Piece::ROOK});
    board.setField(B, 8, ChessPiece{Color::BLACK, Piece::KNIGHT});
    board.setField(C, 8, ChessPiece{Color::BLACK, Piece::BISHOP});
    board.setField(D, 8, ChessPiece{Color::BLACK, Piece::QUEEN});
    board.setField(E, 8, ChessPiece{Color::BLACK, Piece::KING});
    board.setField(F, 8, ChessPiece{Color::BLACK, Piece::BISHOP});
    board.setField(G, 8, ChessPiece{Color::BLACK, Piece::KNIGHT});
    board.setField(H, 8, ChessPiece{Color::BLACK, Piece::ROOK});

    board.setCastlingRaw(0xF);  // All four castling options still available

    return board;
}

//           8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50
// or        8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8
// currently this assumes a valid FEN string and will fail in case of an invalid one
// Todo: Sanity checks and implementation for EnPassant Target
Board BoardFactory::creatBoardFromFEN(std::string fen) {
    Board board;

    std::vector<std::string> fields = base::split(fen, ' ');
    std::vector<std::string> ranks = base::split(fields[0], '/');
    // fmt::print("\nFields: {}", fields);
    // fmt::print("\nRanks: {}", ranks);

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
            board.setField({file, rank}, cp);
            ++file;
        }
        --rank;
    }

    if (fields.size() > 1) {
        Color turn = (fields[1] == "w" ? Color::WHITE : Color::BLACK);
        board.setTurn(turn);

        if (fields[2].find('Q') != std::string::npos) board.setCastling(Board::Castling::WHITE_LONG);
        if (fields[2].find('K') != std::string::npos) board.setCastling(Board::Castling::WHITE_SHORT);
        if (fields[2].find('q') != std::string::npos) board.setCastling(Board::Castling::BLACK_LONG);
        if (fields[2].find('k') != std::string::npos) board.setCastling(Board::Castling::BLACK_SHORT);

        if (fields[3] != "-") {
            board.setEnPassantTarget(getChessFieldFromString(fields[3]));
        }
    }

    return board;
}