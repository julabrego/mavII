#pragma once

#include "Scene.h"
#include "GameState.h"
#include "../domain/PlayerCannon.h"
#include "../domain/Chain.h"
#include <memory>
#include "GameUI.h"

class Game;
class PhysicsWorld;
class Scenario;
class Renderer;
class Level;

class GameplayScene : public Scene
{
public:
	GameplayScene(Game& game, int levelIndex);
	~GameplayScene();

	void HandleInput() override;
	void Update(float deltaTime) override;
	void Draw(Renderer& renderer) override;

private:
	void LoadLevel(int levelIndex);
	void HandlePlayerDeath();

	Game& game;
	GameContext& context;
	int levelIndex = 0;

	std::unique_ptr<PhysicsWorld> physicsWorld;
	std::unique_ptr<Scenario> scenario;
	std::unique_ptr<PlayerCannon> player;
	std::unique_ptr<Level> level;
	Chain chain;

	GameUI ui;
};

