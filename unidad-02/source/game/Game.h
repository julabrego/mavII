#pragma once

#include <memory>
#include "box2d.h"

class PhysicsWorld;
class Renderer;
class CircleEntity;
class RectangleEntity;

class Game
{
	public:
	Game(int screenWidth, int screenHeight);
	~Game();
	void Run();

private:
	std::unique_ptr<PhysicsWorld> physicsWorld;
	std::unique_ptr<Renderer> renderer;
	
	b2Body* circleBody;
	b2Body* groundBody;

	RectangleEntity* groundEntity;
	CircleEntity* circleEntity;

	float impulseAngle;

	void HandleInput();
	void Update(float deltaTime);
	void Draw();
};

