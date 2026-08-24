#pragma once

#include <memory>
#include <vector>
#include "raylib.h"
#include "GameState.h"
#include "GameUI.h"
#include "../domain/PlayerCannon.h"
#include "../domain/WreckingBall.h"
#include "../domain/ChainLink.h"

class PhysicsWorld;
class Renderer;
class CircleEntity;
class RectangleEntity;
class Scenario;
class ContactListener;
class Level;

const float GRAVITY = 9.8f;
const float SHOOT_SPEED = 20.0f;
const float PROJECTILE_RADIUS = 20.0f;
const float LAUNCH_OFFSET = 45.0f;
const float PULL_SPEED = 120.0f;

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
	std::unique_ptr<Level> level;
	
	std::unique_ptr<WreckingBall> wreckingBall;
	std::vector<std::unique_ptr<ChainLink>> projectiles;
	std::vector<b2DistanceJoint*> projectileJoints;
	b2DistanceJoint* cannonRope = nullptr;
	float chainBetweenProjectilesDistance = 5.0f;
	float lastLinkDistance = 0.0f;
	int maxProjectiles = 30;
	float chainTetherMaxLength = 8.0f;

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
	void SpawnProjectile();
	void PullProjectiles(float deltaTime);
	void ConsumeLink();
	void CleanupProjectiles();
	void EnableChainCollisions();
	void DrawDebugJoint(const b2DistanceJoint& joint);
};

