#pragma once

#include <memory>
#include "raylib.h"

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
	std::unique_ptr<PhysicsWorld> physicsWorld;
	std::unique_ptr<Renderer> renderer;
	
	std::unique_ptr<CircleEntity> circleEntity;

	std::unique_ptr<Scenario> scenario;

	float impulseAngle;
	Vector2 initialBallPosition = { 747.0f, 490.0f };

	void HandleInput();
	void RestartBallPosition();
	void Update(float deltaTime);
	void Draw();
	void DrawUI();
};

