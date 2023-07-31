#pragma once

#include "assets.h"
struct GuiState;

void update_logtext(GuiState& state);
void drawLogWindow(const AssetMap& assets, GuiState& state);