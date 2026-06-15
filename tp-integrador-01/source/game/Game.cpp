#include "Game.h"

#include "../core/PhysicsWorld.h"
#include "box2d.h"
#include "raylib.h"
#include "../core/Renderer.h"
#include "../domain/CircleEntity.h"
#include "../domain/RectangleEntity.h"
#include "../domain/Scenario.h"
#include "../core/Colors.h"
#include "../core/PhysicsConstants.h"

const float GRAVITY = 9.8f;
const float IMPULSE_STRENGTH = 50000.0f;

Game::Game(int screenWidth, int screenHeight)
	: physicsWorld(std::make_unique<PhysicsWorld>(GRAVITY))
	, renderer(std::make_unique<Renderer>(COLOR_BACKGROUND))
	, impulseAngle(90.0f)
{
	b2World& world = *physicsWorld->GetWorld();

	scenario = new Scenario(world, screenWidth, screenHeight);
	circleEntity = new CircleEntity(world, initialBallPosition.x, initialBallPosition.y, 25.0f, COLOR_BALL, 1.0f, 0.5f, 0.5f, 2.0f);
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
	float angleInRadians = impulseAngle * DEG2RAD;
	if (IsKeyPressed(KEY_SPACE)) {
		scenario->TriggerSticksAction();
	}
	else if (IsKeyReleased(KEY_SPACE)) {
		scenario->ResetSticks();
	}
	else if (IsKeyPressed(KEY_R)) {
		RestartBallPosition();
	}
	else if (IsKeyPressed(KEY_V)) {
		scenario->ToggleDrawJoints();
	}

	if (IsKeyDown(KEY_DOWN)) {
		scenario->PullPlunger();
	}
	else if (IsKeyReleased(KEY_DOWN)) {
		scenario->ReleasePlunger();
	}
}

void Game::RestartBallPosition()
{
	circleEntity->GetBody()->SetTransform({ initialBallPosition.x * METERS_PER_PIXEL, initialBallPosition.y * METERS_PER_PIXEL }, 0.0f);
	circleEntity->GetBody()->SetLinearVelocity({ 0.0f, 1.0f });
}

void Game::Update(float deltaTime)
{
	scenario->Update(deltaTime);
	circleEntity->Update(deltaTime);

	if (circleEntity->position.y > GetScreenHeight() + 100.0f) {
		RestartBallPosition();
	}
}

void Game::Draw()
{
	renderer->Begin();

	scenario->Render(*renderer);
	circleEntity->Render(*renderer);

	DrawUI();

	renderer->End();
}

void Game::DrawUI() {
	DrawRectangle(10, 116, 265, 450, Fade(BLACK, 1.0f));

	DrawText(R"(Controles:
- 'Abajo' para controlar
	el plunger
- 'Espacio' para activar
	los sticks
- 'R' para reiniciar el
	juego)", 20, 125, 20, RAYWHITE);
}