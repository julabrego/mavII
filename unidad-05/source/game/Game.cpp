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
{
	b2World& world = *physicsWorld->GetWorld();

	scenario = std::make_unique<Scenario>(world, screenWidth, screenHeight);
	player = std::make_unique<Player>(world, 30.0f, 480.0f);

	// TODO: extract: create stuff
	enemy1 = std::make_unique<Enemy>(world, 600.0f, 480.0f);

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
		physicsWorld->Update(deltaTime);
		Update(deltaTime);
		Draw();
	}
}

void Game::HandleInput()
{
	if (!player) return;

	// Player movement input
	player->SetAction(PlayerAction::Left, IsKeyDown(KEY_LEFT));
	player->SetAction(PlayerAction::Right, IsKeyDown(KEY_RIGHT));
	player->SetAction(PlayerAction::Jump, IsKeyDown(KEY_SPACE));

	// TODO: debug
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		b2Vec2 mousePos = { GetMouseX() * METERS_PER_PIXEL, GetMouseY() * METERS_PER_PIXEL };
		player->GetBody()->SetTransform(mousePos, 0.0f);
		player->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}
	// if (IsKeyPressed(KEY_SPACE)) {
	// 	if(context.state == GameState::MainMenu || context.state == GameState::Finished) {
	// 		RestartGame();
	// 	}
	// }

}

void Game::RestartGame()
{
	context.score = 0;
	context.touchedGround = false;
	context.state = GameState::Idle;
}


void Game::Update(float deltaTime)
{
	scenario->Update(deltaTime);

	if (player) {
		player->Update(deltaTime);
		if(player->GetState() == PlayerState::Dead) {
			physicsWorld->GetWorld()->DestroyBody(player->GetBody());
			player.reset();
		}
	}

	if (enemy1) {
		enemy1->Update(deltaTime);
		if(enemy1->GetState() == EnemyState::Dead) {
			physicsWorld->GetWorld()->DestroyBody(enemy1->GetBody());
			enemy1.reset();
		}
	}

	auto& contactListener = physicsWorld->GetContactListener();

	if (contactListener.isRotablePlatformTriggered) {
		scenario->EnablePlatformRotation();
	}
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