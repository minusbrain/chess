#include "board_window.h"

#include <imgui.h>
#include <main.h>

#include "sprite.h"

void drawChessBoardWindow(const AssetMap& assets, GuiState& state, ChessFieldClickHandler onChessFieldClick) {
    static float scale = 2.0f;

    ImGui::Begin("Chess Board");

    if (ImGui::BeginTable("Board", 8)) {
        for (int i = 0; i < 8; ++i) ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 32.0f * scale);

        for (int row = 0; row < 8; row++) {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 34.0f * scale);

            for (int column = 0; column < 8; column++) {
                ImGui::TableSetColumnIndex(column);
                ChessField field{column + 1, 8 - row};
                auto piece = state.game.getBoard().getPieceOnField(field);
                auto fieldState = state.fieldStates[BoardHelper::fieldToIndex(field)];

                if (fieldState == FieldState::NORMAL) {
                    if (row % 2 == 0) {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                               column % 2 == 0 ? ImGui::GetColorU32(white) : ImGui::GetColorU32(black));

                    } else {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                               column % 2 == 0 ? ImGui::GetColorU32(black) : ImGui::GetColorU32(white));
                    }
                } else {
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(FieldStateColors.at(fieldState)));
                    if (state.selectedField && fieldState == FieldState::MOVE_OPTION && !piece) {
                        if (get<Sprite>(assets, "green_marker")->drawToGuiAsButton(fmt::format("bt_{}_{}", row, column), 1)) {
                            onChessFieldClick(field);
                        }
                    }
                }

                if (piece) {
                    if (get<Sprite>(assets, piece_2_asset[*piece])->drawToGuiAsButton(fmt::format("bt_{}_{}", row, column), scale)) {
                        onChessFieldClick(field);
                    }
                }
            }
        }
        ImGui::EndTable();
    }

    // ImGui::ShowMetricsWindow();
    ImGui::End();
}