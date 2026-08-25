#pragma once

#include "Scene.h"
#include "GameState.h"

class Game;
class Renderer;

class MainMenuScene : public Scene
{
public:
	explicit MainMenuScene(Game& game);

	void HandleInput() override;
	void Update(float deltaTime) override;
	void Draw(Renderer& renderer) override;

private:
	Game& game;
	GameContext& context;
};

