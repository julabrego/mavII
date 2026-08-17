#include "Game.h"

#include "../core/PhysicsWorld.h"
#include "box2d.h"
#include "raylib.h"
#include "../core/Renderer.h"
#include "../domain/RectangleEntity.h"
#include "../domain/Scenario.h"
#include "../core/Colors.h"
#include "../core/PhysicsConstants.h"
#include "../core/ContactListener.h"
#include <string>
#include <cmath>

const float GRAVITY = 9.8f;
const float SHOOT_SPEED = 20.0f;
const float PROJECTILE_RADIUS = 10.0f;
const float LAUNCH_OFFSET = 45.0f;
const float PULL_SPEED = 60.0f;

Game::Game(int screenWidth, int screenHeight)
	: physicsWorld(std::make_unique<PhysicsWorld>(GRAVITY))
	, renderer(std::make_unique<Renderer>(COLOR_SKY))
	, screenWidth(screenWidth)
	, screenHeight(screenHeight)
{
	RestartGame();
}

Game::~Game()
{
}

void Game::Run()
{
	while (!WindowShouldClose())
	{
		HandleInput();

		float deltaTime = GetFrameTime();

		if (context.state == GameState::Playing) {
			physicsWorld->Update(deltaTime);
		}

		Update(deltaTime);
		Draw();
	}
}

void Game::HandleInput()
{
	if (context.state == GameState::MainMenu || context.state == GameState::Finished) {
		if (IsKeyPressed(KEY_ENTER)) {
			StartGame();
		}
		return;
	}

	if (player) {
		player->SetAction(PlayerCannonAction::RotateLeft, IsKeyDown(KEY_LEFT));
		player->SetAction(PlayerCannonAction::RotateRight, IsKeyDown(KEY_RIGHT));
		player->SetAction(PlayerCannonAction::MoveUp, IsKeyDown(KEY_UP));
		player->SetAction(PlayerCannonAction::MoveDown, IsKeyDown(KEY_DOWN));
		player->SetAction(PlayerCannonAction::Shoot, IsKeyDown(KEY_SPACE));
		player->SetAction(PlayerCannonAction::Pull, IsKeyDown(KEY_S));
	}

	if (IsKeyPressed(KEY_D)) {
		ToggleDebugMode();
	}
	else if (IsKeyPressed(KEY_R)) {
		RestartGame();
	}

	if (context.debugMode) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			float mouseX = GetMouseX() * METERS_PER_PIXEL;
			float mouseY = GetMouseY() * METERS_PER_PIXEL;
			player->TeleportTo(mouseX, mouseY);
		}
	}
}

void Game::StartGame()
{
	RestartGame();
	context.state = GameState::Playing;
}

void Game::RestartGame()
{
	physicsWorld = std::make_unique<PhysicsWorld>(GRAVITY);
	b2World& world = *physicsWorld->GetWorld();

	context = GameContext{};
	projectiles.clear();
	projectileJoints.clear();
	cannonRope = nullptr;

	scenario = std::make_unique<Scenario>(world, context, screenWidth, screenHeight);
	player = std::make_unique<PlayerCannon>(world, context, 30.0f, 480.0f);

	wreckingBall.reset();
}

void Game::ToggleDebugMode()
{
	context.debugMode = !context.debugMode;
}

void Game::Update(float deltaTime)
{
	auto& contactListener = physicsWorld->GetContactListener();

	scenario->Update(deltaTime);

	if (player) {
		player->Update(deltaTime);

		if (player->GetPullRequested()) {
			PullProjectiles(deltaTime);
		}

		if (!player->GetPullRequested() && player->GetShootRequested()) {
			SpawnProjectile();
		}
	}

	// Wrecking ball + chain
	if(wreckingBall) {
		wreckingBall->Update(deltaTime);
	}

	for (auto& projectile : projectiles) {
		projectile->Update(deltaTime);
	}
	// ---

	CleanupProjectiles();
	HandlePlayerDeath();

	contactListener.ClearFrameEvents();
}

void Game::SpawnProjectile()
{
	if (!player) return;

	float angleRad = player->GetBody()->GetAngle();
	float spawnX = player->GetBody()->GetPosition().x * PIXELS_PER_METER + cosf(angleRad) * LAUNCH_OFFSET;
	float spawnY = player->GetBody()->GetPosition().y * PIXELS_PER_METER + sinf(angleRad) * LAUNCH_OFFSET;

	b2World& world = *physicsWorld->GetWorld();

	if (!wreckingBall) {
		auto ball = std::make_unique<WreckingBall>(world, spawnX, spawnY, PROJECTILE_RADIUS);
		ball->Launch(angleRad, SHOOT_SPEED);
		wreckingBall = std::move(ball);
	}

	b2Vec2 cannonPosition = player->GetPosition();
	bool canSpawnProjectile = true;
	b2Vec2 ballPosition = projectiles.size() == 0 ? wreckingBall->GetBody()->GetPosition() : projectiles.back()->GetBody()->GetPosition();
	float distance = b2Distance(cannonPosition, ballPosition);

	canSpawnProjectile = (distance > chainBetweenProjectilesDistance * METERS_PER_PIXEL);
	
	if (canSpawnProjectile) {
		if (projectiles.size() >= maxProjectiles) {
			return;
		}

		auto ball = std::make_unique<ChainLink>(world, spawnX, spawnY, angleRad);
		ball->Launch(angleRad, SHOOT_SPEED);
		projectiles.push_back(std::move(ball));

		float linkGapInMeters = chainBetweenProjectilesDistance * METERS_PER_PIXEL;
		b2DistanceJointDef distanceJointDef;

		distanceJointDef.bodyA = projectiles.size() == 1 ? wreckingBall->GetBody() : projectiles[projectiles.size() - 2]->GetBody();
		distanceJointDef.bodyB = projectiles.back()->GetBody();

		float borderA = projectiles.size() == 1 ? 0.0f : -(LINK_WIDTH / 2.0f) * METERS_PER_PIXEL;
		float borderB = (LINK_WIDTH / 2.0f) * METERS_PER_PIXEL;
		
		distanceJointDef.localAnchorA.Set(borderA, 0.0f);
		distanceJointDef.localAnchorB.Set(borderB, 0.0f);
		distanceJointDef.length = linkGapInMeters;
		distanceJointDef.minLength = 0.0f;
		distanceJointDef.maxLength = linkGapInMeters;
		projectileJoints.push_back(static_cast<b2DistanceJoint*>(world.CreateJoint(&distanceJointDef)));

		if (cannonRope) {
			world.DestroyJoint(cannonRope);
			cannonRope = nullptr;
		}

		b2DistanceJointDef tetherDef;
		tetherDef.bodyA = player->GetBody();
		tetherDef.bodyB = projectiles.back()->GetBody();
		tetherDef.localAnchorA.Set(LAUNCH_OFFSET * METERS_PER_PIXEL, 0.0f);
		tetherDef.localAnchorB.Set(-(LINK_WIDTH / 2.0f) * METERS_PER_PIXEL, 0.0f);
		tetherDef.length = chainTetherMaxLength * METERS_PER_PIXEL;
		tetherDef.minLength = 0.0f;
		tetherDef.maxLength = chainTetherMaxLength * METERS_PER_PIXEL;
		cannonRope = static_cast<b2DistanceJoint*>(world.CreateJoint(&tetherDef));

		if (projectiles.size() >= maxProjectiles) {
			EnableChainCollisions();
		}
	}
	
}

void Game::PullProjectiles(float deltaTime)
{
	if (!player || !wreckingBall) return;

	if (!cannonRope) {
		return;
	}

	float currentMaxPx = cannonRope->GetMaxLength() * PIXELS_PER_METER;
	float newMaxPx = currentMaxPx - PULL_SPEED * deltaTime;

	if (newMaxPx <= 0.0f) {
		ConsumeLink();
	}
	else {
		cannonRope->SetMaxLength(newMaxPx * METERS_PER_PIXEL);
	}
}

void Game::ConsumeLink()
{
	if (!player || !wreckingBall) return;

	b2World& world = *physicsWorld->GetWorld();

	if (projectiles.empty()) {
		physicsWorld->DestroyBody(wreckingBall->GetBody());
		wreckingBall.reset();
		return;
	}

	physicsWorld->DestroyBody(projectiles.back()->GetBody());
	projectiles.pop_back();
	if (!projectileJoints.empty()) {
		projectileJoints.pop_back();
	}
	cannonRope = nullptr;

	if (projectiles.empty()) {
		physicsWorld->DestroyBody(wreckingBall->GetBody());
		wreckingBall.reset();
		return;
	}

	b2DistanceJointDef tetherDef;
	tetherDef.bodyA = player->GetBody();
	tetherDef.bodyB = projectiles.back()->GetBody();
	tetherDef.localAnchorA.Set(LAUNCH_OFFSET * METERS_PER_PIXEL, 0.0f);
	tetherDef.localAnchorB.Set(-(LINK_WIDTH / 2.0f) * METERS_PER_PIXEL, 0.0f);

	b2Vec2 anchorA = player->GetBody()->GetWorldPoint(tetherDef.localAnchorA);
	b2Vec2 anchorB = projectiles.back()->GetBody()->GetWorldPoint(tetherDef.localAnchorB);
	float actualLength = b2Distance(anchorA, anchorB);

	tetherDef.length = actualLength;
	tetherDef.minLength = 0.0f;
	tetherDef.maxLength = actualLength;
	cannonRope = static_cast<b2DistanceJoint*>(world.CreateJoint(&tetherDef));
}

void Game::EnableChainCollisions()
{
	b2Filter filter;
	filter.groupIndex = 0;

	if (wreckingBall) {
		wreckingBall->GetBody()->GetFixtureList()->SetFilterData(filter);
	}
	for (auto& link : projectiles) {
		link->GetBody()->GetFixtureList()->SetFilterData(filter);
	}
}

void Game::CleanupProjectiles()
{
	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		float x = (*it)->GetBody()->GetPosition().x * PIXELS_PER_METER;
		float y = (*it)->GetBody()->GetPosition().y * PIXELS_PER_METER;
		if (x < -100.0f || x > screenWidth + 100.0f || y < -100.0f || y > screenHeight + 100.0f) {
			physicsWorld->DestroyBody((*it)->GetBody());
			it = projectiles.erase(it);
			projectileJoints.clear();
			cannonRope = nullptr;
		}
		else {
			++it;
		}
	}
}

void Game::HandlePlayerDeath()
{
	if (!player) return;

	float playerY = player->GetBody()->GetPosition().y * PIXELS_PER_METER;
	float playerHalfHeight = 30.0f;
	if (playerY > screenHeight + playerHalfHeight && context.state == GameState::Playing) {
		player->Die();
	}

	if (player->GetState() == PlayerCannonState::Dead && context.state == GameState::Playing) {
		physicsWorld->DestroyBody(player->GetBody());
		cannonRope = nullptr;
		player.reset();

		context.state = GameState::Finished;
		context.finishState = GameFinishState::Lost;
	}
}

void Game::Draw()
{
	renderer->Begin();

	scenario->Render(*renderer);

	// Wrecking ball + chain
	if(wreckingBall) {
		wreckingBall->Render(*renderer);
	}
	for (auto& projectile : projectiles) {
		projectile->Render(*renderer);
	}
	// ---

	if (player) {
		player->Render(*renderer);
	}

	if (context.debugMode) {
		for (auto& joint : projectileJoints) {
			DrawDebugJoint(*joint);
		}
		if (cannonRope) {
			DrawDebugJoint(*cannonRope);
		}
	}

	ui.Draw(*renderer, context);

	renderer->End();
}

void Game::DrawDebugJoint(const b2DistanceJoint& joint)
{
	b2Vec2 anchorA = joint.GetAnchorA();
	b2Vec2 anchorB = joint.GetAnchorB();
	DrawCircle(static_cast<int>(anchorA.x * PIXELS_PER_METER), static_cast<int>(anchorA.y * PIXELS_PER_METER), 3.0f, RED);
	DrawCircle(static_cast<int>(anchorB.x * PIXELS_PER_METER), static_cast<int>(anchorB.y * PIXELS_PER_METER), 3.0f, RED);
}
