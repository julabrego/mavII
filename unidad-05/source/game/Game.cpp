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

const float GRAVITY = 9.8f;
const float SPRING_STRENGTH = -17.5f;
const float IMPULSE_STRENGTH = 30.0f;

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

		if(context.state == GameState::Playing) {
			physicsWorld->Update(deltaTime);
		}

		Update(deltaTime);
		Draw();
	}
}

void Game::HandleInput()
{
	if(context.state == GameState::MainMenu || context.state == GameState::Finished) {
		if (IsKeyPressed(KEY_ENTER)) {
			StartGame();
		}
		return;
	}

	if (player) {
		// Player movement input
		player->SetAction(PlayerAction::Left, IsKeyDown(KEY_LEFT));
		player->SetAction(PlayerAction::Right, IsKeyDown(KEY_RIGHT));
		player->SetAction(PlayerAction::Jump, IsKeyDown(KEY_SPACE));
	}

	if (
		IsKeyPressed(KEY_D)) {
		ToggleDebugMode();
	} else if (IsKeyPressed(KEY_R)) {
		RestartGame();
	}

	if (context.debugMode) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			b2Vec2 mousePos = { GetMouseX() * METERS_PER_PIXEL, GetMouseY() * METERS_PER_PIXEL };
			player->GetBody()->SetTransform(mousePos, 0.0f);
			player->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
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

	scenario = std::make_unique<Scenario>(world, context, screenWidth, screenHeight);
	player = std::make_unique<Player>(world, context, 30.0f, 480.0f);
	enemy1 = std::make_unique<Enemy>(world, context, 600.0f, 480.0f);
}

void Game::ToggleDebugMode()
{
	context.debugMode = !context.debugMode;
}


void Game::Update(float deltaTime)
{
	auto& contactListener = physicsWorld->GetContactListener();

	if (contactListener.groundContactDelta > 0 && player) {
		for (int i = 0; i < contactListener.groundContactDelta; i++) {
			player->IncrementGroundContacts();
		}
	}
	else if (contactListener.groundContactDelta < 0 && player) {
		for (int i = 0; i > contactListener.groundContactDelta; i--) {
			player->DecrementGroundContacts();
		}
	}

	scenario->Update(deltaTime);

	if (player) {
		player->Update(deltaTime);
	}

	if (enemy1) {
		enemy1->Update(deltaTime);
	}

	if (contactListener.playerVsEnemyContact && player && enemy1) {
		if (player->GetState() != PlayerState::Falling) {
			player->TakeDamage();
		}
	}

	if (contactListener.playerStompContact && player && enemy1) {
		player->Bounce();
		enemy1->TakeDamage();
	}

	if (player) {
		float playerY = player->GetBody()->GetPosition().y * PIXELS_PER_METER;
		float playerHalfHeight = 30.0f;
		if (playerY > screenHeight + playerHalfHeight && context.state == GameState::Playing) {
			player->Die();
		}

		if (player->GetState() == PlayerState::Dead && context.state == GameState::Playing) {
			physicsWorld->DestroyBody(player->GetBody());
			player.reset();

			context.state = GameState::Finished;
			context.finishState = GameFinishState::Lost;
		}
	}

	if (enemy1) {
		if (enemy1->GetState() == EnemyState::Dead) {
			physicsWorld->DestroyBody(enemy1->GetBody());
			enemy1.reset();
		}
	}

	if (contactListener.isRotablePlatformTriggered) {
		scenario->EnablePlatformRotation();
	}

	if (contactListener.playerReachedFinishSensor && context.state == GameState::Playing) {
		context.state = GameState::Finished;
		context.finishState = GameFinishState::Won;
	}

	contactListener.ClearFrameEvents();
}

void Game::Draw()
{
	renderer->Begin();

	scenario->Render(*renderer);

	if(player) {
		player->Render(*renderer);
	}

	if (enemy1) {
		enemy1->Render(*renderer);
	}

	ui.Draw(*renderer, context);

	renderer->End();
}