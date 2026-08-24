#pragma once

#include "Scene.h"
#include "GameState.h"
#include "GameUI.h"
#include "../domain/PlayerCannon.h"
#include "../domain/WreckingBall.h"
#include "../domain/ChainLink.h"
#include <memory>
#include <vector>

class PhysicsWorld;
class Renderer;
class Scenario;
class Level;
class Game;

class GameplayScene : public Scene {
public:
	GameplayScene(Game& game, int levelIndex);
	~GameplayScene();

	void HandleInput() override;
	void Update(float deltaTime) override;
	void Draw(Renderer& renderer) override;

private:
	void LoadLevel(int levelIndex);
	void SpawnProjectile();
	void PullProjectiles(float deltaTime);
	void ConsumeLink();
	void CleanupProjectiles();
	void EnableChainCollisions();
	void HandlePlayerDeath();
	void DrawDebugJoint(const b2DistanceJoint& joint);

	Game& game;
	GameContext& context;
	int levelIndex = 0;

	std::unique_ptr<PhysicsWorld> physicsWorld;
	std::unique_ptr<Scenario> scenario;
	std::unique_ptr<PlayerCannon> player;
	std::unique_ptr<Level> level;
	std::unique_ptr<WreckingBall> wreckingBall;
	std::vector<std::unique_ptr<ChainLink>> projectiles;
	std::vector<b2DistanceJoint*> projectileJoints;
	b2DistanceJoint* cannonRope = nullptr;

	GameUI ui;
};
