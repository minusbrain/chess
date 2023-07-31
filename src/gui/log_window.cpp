#include "log_window.h"

#include <imgui.h>
#include <main.h>

void update_logtext(GuiState& state) {
    state.log_text = fmt::format("It's {}'s move ({})\n", state.game.getMovingPlayer().getName(), state.game.getBoard().whosTurnIsIt());
    state.log_text += fmt::format("All available moves: {}\n", state.validMoves);
    if (state.selectedField) {
        state.log_text +=
            fmt::format("Selected {:u} on field {:u} has the following potential moves: {}\n",
                        state.game.getBoard().getPieceOnField(state.selectedField.value()).value_or(ChessPiece{Color::WHITE, Piece::DECOY}),
                        state.selectedField.value(), state.validMovesOfSelectedPiece);
    }
    state.log_text += "Previous moves:\n";
    int i = 0;
    auto endIt = state.game.getProgress().end();
    for (auto move = state.game.getProgress().begin(); move != endIt; ++move) {
        std::shared_ptr<Move> firstMove = (*move)->moveToNext;
        if (!firstMove) continue;
        ++move;
        if (move != endIt) {
            std::shared_ptr<Move> secondMove = (*move)->moveToNext;
            if (secondMove) {
                state.log_text += fmt::format("{}: {} {}", ++i, *firstMove, *secondMove);
                if (i % 4 == 0)
                    state.log_text += "\n";
                else
                    state.log_text += "  ";
                continue;
            }
        }
        state.log_text += fmt::format("{}: {}", ++i, *firstMove);
    }
}

void drawLogWindow(const AssetMap& /*assets*/, GuiState& state) {
    ImGui::Begin("Chess Log");
    // Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
    ImGui::TextWrapped("%s", state.log_text.c_str());
    ImGui::Spacing();
    ImGui::End();
}