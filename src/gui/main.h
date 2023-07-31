#pragma once
#include <chess.h>
#include <imgui.h>

#include <map>

extern std::map<ChessPiece, std::string> piece_2_asset;

enum FieldState { NORMAL = 0, SELECTED_HAS_MOVES, SELECTED_NO_MOVES, MOVE_OPTION, CHECK, CHECK_MATE, LAST_MOVE };
struct GuiState {
    GuiState(ChessPlayer& white, ChessPlayer& black) : game(white, black) {}

    // Game State
    ChessGame game;
    bool runGame = true;

    // GUI State
    std::optional<ChessField> selectedField;
    std::optional<ChessField> lastMove;
    std::vector<Move> validMoves;
    std::vector<Move> validMovesOfSelectedPiece;
    bool board_state_changed = true;

    bool show_demo_window = false;
    bool show_chess = true;
    bool show_chess_log = true;
    std::array<FieldState, 64> fieldStates;

    std::string log_text;
};

static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static const ImVec4 orange = ImVec4(239.0f / 255, 163.0f / 255, 10.0f / 255, 1.0f);
static const ImVec4 red = ImVec4(0.8f, 0.0f, 0.0f, 1.0f);
static const ImVec4 light_green = ImVec4(120.0f / 255, 1.0f, 120.0f / 255, 1.0f);
static const ImVec4 green = ImVec4(0.0f, 0.8f, 0.0f, 1.0f);
static const ImVec4 white = ImVec4(0.4f, 0.4f, 0.8f, 1.0f);
static const ImVec4 black = ImVec4(0.0f, 0.0f, 0.2f, 1.0f);
static const ImVec4 grey = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

const extern std::map<FieldState, ImVec4> FieldStateColors;
