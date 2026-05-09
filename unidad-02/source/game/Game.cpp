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

	Scenario scenario(world, screenWidth, screenHeight);

	groundEntity = new RectangleEntity(world, 0.0f, screenHeight - 60.0f, screenWidth, 40.0f, COLOR_GROUND);
	circleEntity = new CircleEntity(world, screenWidth / 2.0f, screenHeight - 80.0f, 30.0f, DARKBLUE, 0.5f, 0.5f, 0.5f, 2.0f, BLACK);
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
		b2Vec2 impulse(IMPULSE_STRENGTH * cosf(angleInRadians), -IMPULSE_STRENGTH * sinf(angleInRadians));
		circleEntity->GetBody()->ApplyLinearImpulseToCenter(impulse, true);
	}

	if (IsKeyDown(KEY_LEFT)) {
		impulseAngle += 1.0f;
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		impulseAngle -= 1.0f;
	}
}

void Game::Update(float deltaTime)
{
	groundEntity->Update(deltaTime);
	circleEntity->Update(deltaTime);
}

void Game::Draw()
{
	renderer->Begin();

	groundEntity->Render(*renderer);
	circleEntity->Render(*renderer);

	DrawUI();

	renderer->End();
}

void Game::DrawUI() {
	Vector2 lineStartPosition = {
	circleEntity->GetBody()->GetPosition().x,
	circleEntity->GetBody()->GetPosition().y
	};

	Vector2 lineEndPosition = {
		circleEntity->GetBody()->GetPosition().x + 50.0f * cosf(impulseAngle * DEG2RAD),
		circleEntity->GetBody()->GetPosition().y - 50.0f * sinf(impulseAngle * DEG2RAD)
	};

	DrawLineEx(lineStartPosition, lineEndPosition, 2.0f, GREEN);
	DrawRectangle(90, 40, 820, 70, Fade(BLACK, 0.18f));

	renderer->DrawCenteredText("Aplicacion de fuerzas en Box2D", 28, 60, COLOR_PRIMARY);
	renderer->DrawCenteredText("Flechas para cambiar direccion. Espacio para aplicar impulso", 20, 550, RAYWHITE);
}