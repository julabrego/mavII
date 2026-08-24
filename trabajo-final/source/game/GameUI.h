#pragma once

#include "GameState.h"
#include "raylib.h"

class Renderer;

class GameUI {
public:
	void Draw(Renderer& renderer, const GameContext& context,
		int shotsLeft = 0, int currentHeight = 0, int heightTarget = 0);
};
