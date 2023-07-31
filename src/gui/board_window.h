#pragma once

#include <chess.h>

#include <functional>

#include "assets.h"
struct GuiState;

using ChessFieldClickHandler = std::function<void(ChessField)>;

void drawChessBoardWindow(const AssetMap& assets, GuiState& state, ChessFieldClickHandler onChessFieldClick);