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
		player->SetAction(PlayerCannonAction::Shoot, IsKeyPressed(KEY_SPACE));
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

	scenario = std::make_unique<Scenario>(world, context, screenWidth, screenHeight);
	player = std::make_unique<PlayerCannon>(world, context, 30.0f, 480.0f);
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

		if (player->ConsumeShoot()) {
			SpawnProjectile();
		}
	}

	for (auto& projectile : projectiles) {
		projectile->Update(deltaTime);
	}

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
	auto ball = std::make_unique<WreckingBall>(world, spawnX, spawnY, PROJECTILE_RADIUS);
	ball->Launch(angleRad, SHOOT_SPEED);
	projectiles.push_back(std::move(ball));
}

void Game::CleanupProjectiles()
{
	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		float x = (*it)->GetBody()->GetPosition().x * PIXELS_PER_METER;
		float y = (*it)->GetBody()->GetPosition().y * PIXELS_PER_METER;
		if (x < -100.0f || x > screenWidth + 100.0f || y < -100.0f || y > screenHeight + 100.0f) {
			physicsWorld->DestroyBody((*it)->GetBody());
			it = projectiles.erase(it);
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
		player.reset();

		context.state = GameState::Finished;
		context.finishState = GameFinishState::Lost;
	}
}

void Game::Draw()
{
	renderer->Begin();

	scenario->Render(*renderer);

	for (auto& projectile : projectiles) {
		projectile->Render(*renderer);
	}

	if (player) {
		player->Render(*renderer);
	}

	ui.Draw(*renderer, context);

	renderer->End();
}
