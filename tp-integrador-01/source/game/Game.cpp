#include "Game.h"

#include "../core/PhysicsWorld.h"
#include "box2d.h"
#include "raylib.h"
#include "../core/Renderer.h"
#include "../domain/CircleEntity.h"
#include "../domain/RectangleEntity.h"
#include "../domain/Scenario.h"
#include "../core/Colors.h"

const float GRAVITY = 9.8f;
const float IMPULSE_STRENGTH = 50000.0f;

Game::Game(int screenWidth, int screenHeight)
	: physicsWorld(std::make_unique<PhysicsWorld>(GRAVITY))
	, renderer(std::make_unique<Renderer>(COLOR_BACKGROUND))
	, impulseAngle(90.0f)
{
	b2World& world = *physicsWorld->GetWorld();

	scenario = new Scenario(world, screenWidth, screenHeight);
	circleEntity = new CircleEntity(world, 747.0f, 300.0f, 25.0f, COLOR_BALL, 1.0f, 0.5f, 0.5f, 2.0f);
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
		//scenario->InteractWithPlatform();
	}else if (IsKeyPressed(KEY_V)) {
		scenario->ToggleDrawJoints();
	}
}

void Game::Update(float deltaTime)
{
	scenario->Update(deltaTime);
	circleEntity->Update(deltaTime);
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
	DrawRectangle(90, 525, 820, 70, Fade(BLACK, 0.18f));

	renderer->DrawCenteredText("'Espacio' para interactuar con el escenario", 20, 535, RAYWHITE);
	renderer->DrawCenteredText("'V' para mostrar/ocultar joints", 20, 565, RAYWHITE);
}