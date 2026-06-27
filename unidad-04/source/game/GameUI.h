#pragma once

#include "GameState.h"
#include "raylib.h"

class Renderer;

class GameUI {
public:
	void Draw(Renderer& renderer, const GameContext& context);
};
