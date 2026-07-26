#pragma once

#include <memory>
#include "raylib.h"
#include "GameState.h"
#include "GameUI.h"
#include "../domain/PlayerCannon.h"

class PhysicsWorld;
class Renderer;
class CircleEntity;
class RectangleEntity;
class Scenario;
class ContactListener;

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
	
	std::unique_ptr<Scenario> scenario;
	std::unique_ptr<PlayerCannon> player;

	GameUI ui;

	int screenWidth;
	int screenHeight;

	void HandleInput();
	void StartGame();
	void RestartGame();
	void Update(float deltaTime);
	void Draw();
	void ToggleDebugMode();

	void HandlePlayerDeath();
};

