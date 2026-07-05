#pragma once

#include <memory>
#include "raylib.h"
#include "GameState.h"
#include "GameUI.h"
#include "../domain/Player.h"

class PhysicsWorld;
class Renderer;
class CircleEntity;
class RectangleEntity;
class Scenario;

class Game
{
	public:
	Game(int screenWidth, int screenHeight);
	~Game();
	void Run();

private:
	GameContext context;

	std::unique_ptr<PhysicsWorld> physicsWorld;
	std::unique_ptr<Renderer> renderer;
	
	std::unique_ptr<CircleEntity> circleEntity;

	std::unique_ptr<Scenario> scenario;
	std::unique_ptr<Player> player;

	GameUI ui;

	Vector2 initialBallPosition = { 62.0f, GetScreenHeight() / 2.0f };

	void HandleInput();
	void RestartGame();
	void Update(float deltaTime);
	void Draw();
};

