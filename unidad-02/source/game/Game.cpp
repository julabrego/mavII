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
Color COLOR_SECONDARY = DARKPURPLE;
Color COLOR_GROUND = Fade(DARKGREEN, 0.7f);

Game::Game(int screenWidth, int screenHeight)
	: physicsWorld(std::make_unique<PhysicsWorld>(GRAVITY))
	, renderer(std::make_unique<Renderer>(COLOR_BACKGROUND))
	, impulseAngle(0.0f)
{
	b2World& world = *physicsWorld->GetWorld();

	Renderer renderer(COLOR_BACKGROUND);

	Scenario scenario(world, screenWidth, screenHeight);

	groundBody = scenario.GetGroundBody();
	b2Vec2 groundPosition = groundBody->GetPosition();
	groundEntity = new RectangleEntity(groundPosition.x, groundPosition.y, screenWidth, 40.0f, COLOR_GROUND);

	b2BodyDef circleDef;
	circleDef.type = b2_dynamicBody;
	circleDef.position.Set(GetScreenWidth() / 2, 50.0f);

	circleBody = world.CreateBody(&circleDef);

	b2CircleShape circleShape;
	circleShape.m_radius = 30.0f;

	b2FixtureDef circleFixture;
	circleFixture.shape = &circleShape;
	circleFixture.density = 0.5f;
	circleFixture.friction = 0.5f;
	circleFixture.restitution = 0.5f;

	circleBody->CreateFixture(&circleFixture);

	circleEntity = new CircleEntity(circleBody->GetPosition().x, circleBody->GetPosition().y, circleShape.m_radius, DARKBLUE, 2.0f, BLACK);
}

Game::~Game()
{
}

void Game::Run()
{
	while (!WindowShouldClose())
	{
		HandleInput();

		// Avanzar simulaci�n
		float deltaTime = GetFrameTime();
		physicsWorld->Update(deltaTime);

		renderer->Begin();

		groundEntity->Update(groundBody, deltaTime, *renderer);
		circleEntity->Update(circleBody, deltaTime, *renderer);

		Vector2 lineStartPosition = {
			circleBody->GetPosition().x,
			circleBody->GetPosition().y
		};

		Vector2 lineEndPosition = {
			circleBody->GetPosition().x + 50.0f * cosf(impulseAngle * DEG2RAD),
			circleBody->GetPosition().y - 50.0f * sinf(impulseAngle * DEG2RAD)
		};

		DrawLineEx(lineStartPosition, lineEndPosition, 2.0f, GREEN);

		// Panel superior
		DrawRectangle(90, 40, 820, 70, Fade(BLACK, 0.18f));
		renderer->DrawCenteredText("Aplicacion de fuerzas en Box2D", 28, 60, COLOR_PRIMARY);

		// Pie
		renderer->DrawCenteredText("Flechas para cambiar direccion. Espacio para aplicar impulso", 20, 550, RAYWHITE);

		renderer->End();
	}
}

void Game::HandleInput()
{
	float angleInRadians = impulseAngle * DEG2RAD;
	if (IsKeyPressed(KEY_SPACE)) {
		b2Vec2 impulse(IMPULSE_STRENGTH * cosf(angleInRadians), -IMPULSE_STRENGTH * sinf(angleInRadians));
		circleBody->ApplyLinearImpulseToCenter(impulse, true);
	}

	if (IsKeyDown(KEY_LEFT)) {
		impulseAngle += 1.0f;
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		impulseAngle -= 1.0f;
	}
}