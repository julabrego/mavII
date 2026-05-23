#include "Game.h"

#include "../core/PhysicsWorld.h"
#include "box2d.h"
#include "raylib.h"
#include "../core/Renderer.h"
#include "../domain/CircleEntity.h"
#include "../domain/RectangleEntity.h"
#include "../domain/Scenario.h"

const float GRAVITY = 9.8f;
const float IMPULSE_STRENGTH = 50000.0f;

Color COLOR_BACKGROUND = { 110, 100, 215, 255 };
Color COLOR_PRIMARY = RAYWHITE;
Color COLOR_GROUND = Fade(DARKGREEN, 0.7f);

Game::Game(int screenWidth, int screenHeight)
	: physicsWorld(std::make_unique<PhysicsWorld>(GRAVITY))
	, renderer(std::make_unique<Renderer>(COLOR_BACKGROUND))
	, impulseAngle(90.0f)
{
	b2World& world = *physicsWorld->GetWorld();

	scenario = new Scenario(world, screenWidth, screenHeight);
	circleEntity = new CircleEntity(world, 120.0f, 100.0f, 30.0f, DARKBLUE, 1.0f, 0.5f, 0.5f, 2.0f, BLACK);
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
		scenario->InteractWithPlatform();
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

	// TODO: skipping UI
	//DrawUI();

	renderer->End();
}

void Game::DrawUI() {
	DrawRectangle(90, 40, 820, 70, Fade(BLACK, 0.18f));

	renderer->DrawCenteredText("Aplicacion de fuerzas en Box2D", 28, 60, COLOR_PRIMARY);
	renderer->DrawCenteredText("Flechas para cambiar direccion. Espacio para aplicar impulso", 20, 550, RAYWHITE);
}