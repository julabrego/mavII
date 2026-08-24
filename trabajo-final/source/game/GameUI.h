#pragma once

#include "GameState.h"
#include "raylib.h"

class Renderer;

struct HudInfo {
	int levelNumber = 1;
	bool isLastLevel = false;
	int shotsLeft = 0;
	int currentHeight = 0;
	int heightTarget = 0;
};

class GameUI {
public:
	void Draw(Renderer& renderer, const GameContext& context, const HudInfo& hud);
};
