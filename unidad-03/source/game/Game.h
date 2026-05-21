#pragma once

#include <memory>

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
	
	CircleEntity* circleEntity;

	Scenario* scenario;

	float impulseAngle;

	void HandleInput();
	void Update(float deltaTime);
	void Draw();
	void DrawUI();
};

