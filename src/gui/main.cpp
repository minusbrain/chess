#include "main.h"

#include <SDL.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <stdio.h>

#include <array>
#include <exception>
#include <optional>
#include <string>

#include "assets.h"
#include "board_window.h"
#include "human_gui_player.h"
#include "log_window.h"
#include "sdl_helper.h"
#include "sprite.h"
#include "texture.h"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

std::map<ChessPiece, std::string> piece_2_asset{
    {{Color::WHITE, Piece::PAWN}, "white_pawn"},     {{Color::BLACK, Piece::PAWN}, "black_pawn"},
    {{Color::WHITE, Piece::ROOK}, "white_rook"},     {{Color::BLACK, Piece::ROOK}, "black_rook"},
    {{Color::WHITE, Piece::KNIGHT}, "white_knight"}, {{Color::BLACK, Piece::KNIGHT}, "black_knight"},
    {{Color::WHITE, Piece::BISHOP}, "white_bishop"}, {{Color::BLACK, Piece::BISHOP}, "black_bishop"},
    {{Color::WHITE, Piece::QUEEN}, "white_queen"},   {{Color::BLACK, Piece::QUEEN}, "black_queen"},
    {{Color::WHITE, Piece::KING}, "white_king"},     {{Color::BLACK, Piece::KING}, "black_king"}};

const std::map<FieldState, ImVec4> FieldStateColors{{FieldState::SELECTED_HAS_MOVES, green}, {FieldState::SELECTED_NO_MOVES, orange},
                                                    {FieldState::MOVE_OPTION, light_green},  {FieldState::CHECK, orange},
                                                    {FieldState::CHECK_MATE, red},           {FieldState::LAST_MOVE, grey}};

void doMove(GuiState& state, ChessField start, ChessField end) {
    for (auto& move : state.validMoves) {
        if (move.getStartField() == start && move.getEndField() == end) {
            state.game.doAsyncMove(state.game.getBoard().whosTurnIsIt(), move);
            state.lastMove = end;
            state.selectedField = std::nullopt;
            state.board_state_changed = true;
            return;
        }
    }
}

void update_state(GuiState& state) {
    if (!state.runGame) return;

    if (state.board_state_changed) {
        state.fieldStates.fill(FieldState::NORMAL);
        const Board& board = state.game.getBoard();
        Color turn = board.whosTurnIsIt();
        if (ChessRules::isCheck(board) || ChessRules::isCheckMate(board)) {
            std::optional<ChessField> kingFieldOpt = board.findFirstPiece([&turn](ChessPiece cp) {
                return cp == ChessPiece{turn, Piece::KING};
            });
            assert(kingFieldOpt.has_value());
            ChessField kingField = kingFieldOpt.value();
            state.fieldStates[BoardHelper::fieldToIndex(kingField)] =
                ChessRules::isCheckMate(board) ? FieldState::CHECK_MATE : FieldState::CHECK;
        }
        if (state.lastMove) {
            state.fieldStates[BoardHelper::fieldToIndex(state.lastMove.value())] = FieldState::LAST_MOVE;
        }
        if (state.selectedField) {
            state.validMovesOfSelectedPiece = ChessRules::getAllValidMoves(board, state.selectedField.value());
            state.fieldStates[BoardHelper::fieldToIndex(state.selectedField.value())] =
                state.validMovesOfSelectedPiece.size() > 0 ? FieldState::SELECTED_HAS_MOVES : FieldState::SELECTED_NO_MOVES;
            for (auto& move : state.validMovesOfSelectedPiece) {
                state.fieldStates[BoardHelper::fieldToIndex(move.getEndField())] = FieldState::MOVE_OPTION;
            }
        }
        state.validMoves = ChessRules::getAllValidMoves(board);

        if (state.show_chess_log) {
            update_logtext(state);
        }

        state.board_state_changed = false;
    }

    if (state.game.getState() == ChessGame::State::FINISHED) {
        state.runGame = false;
        return;
    }

    if (state.game.getMovingPlayer().useGetMove()) {
        Move move = state.game.getMovingPlayer().getMove(state.game.getBoard(), state.validMoves);
        doMove(state, move.getStartField(), move.getEndField());
    }
}

void draw_gui(SDL_Renderer* renderer, const AssetMap& assets, GuiState& state) {
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    auto chessFieldClickHandler = [&state](ChessField field) {
        auto piece = state.game.getBoard().getPieceOnField(field);
        auto fieldState = state.fieldStates[BoardHelper::fieldToIndex(field)];

        if (fieldState != FieldState::NORMAL) {
            if (state.selectedField && fieldState == FieldState::MOVE_OPTION && !piece) {
                doMove(state, state.selectedField.value(), field);
            }
        }

        if (piece) {
            if (state.selectedField && state.selectedField.value() == field) {
                state.selectedField = std::nullopt;
                state.board_state_changed = true;
            } else if (state.selectedField && state.selectedField.value() != field && fieldState == FieldState::MOVE_OPTION) {
                doMove(state, state.selectedField.value(), field);
            } else {
                state.selectedField = field;
                state.board_state_changed = true;
            }
        }
    };

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about
    // Dear ImGui!).
    if (state.show_demo_window) ImGui::ShowDemoWindow(&state.show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::Begin("Chess");  // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Available Windows");                         // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &state.show_demo_window);  // Edit bools storing our window open/close state
        ImGui::Checkbox("Chess Board", &state.show_chess);
        ImGui::Checkbox("Chess Log", &state.show_chess_log);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (state.show_chess) {
        drawChessBoardWindow(assets, state, chessFieldClickHandler);
    }

    if (state.show_chess_log) {
        drawLogWindow(assets, state);
    }

    // Rendering
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255),
                           (Uint8)(clear_color.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(renderer);
}

// Main code
int main(int, char**) {
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window =
        SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    // Setup SDL_Renderer instance
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Error creating SDL_Renderer!");
        return -1;
    }
    // SDL_RendererInfo info;
    // SDL_GetRendererInfo(renderer, &info);
    // SDL_Log("Current SDL_Renderer: %s", info.name);

    AssetMap assets;
    try {
        assets = loadAssets("assets/pieces.json", renderer);
    } catch (const std::exception& ex) {
        fmt::println("Could not load assets from assets/pieces.json. Error: {}", ex.what());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    OneMoveDeepBestPositionChessPlayer whitePlayer{"Andreas"};
    HumanGuiPlayer blackPlayer{"Human"};
    GuiState state{whitePlayer, blackPlayer};

    state.fieldStates.fill(FieldState::NORMAL);
    state.game.startAsyncronousGame();

    // Main loop
    bool done = false;
    while (!done) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of
        // the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy
        // of the keyboard data. Generally you may always pass all inputs to dear imgui, and hide them from your application based on those
        // two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        update_state(state);
        draw_gui(renderer, assets, state);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
