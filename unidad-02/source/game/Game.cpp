#include "Game.h"

#include "../core/PhysicsWorld.h"
#include "box2d.h"
#include "raylib.h"

#include "../core/Renderer.h"
#include "../domain/CircleEntity.h"
#include "../domain/RectangleEntity.h"
#include "../domain/Scenario.h"

const float GRAVITY = 9.8f;
float impulseAngle = 0.0f;
float impulseStrength = 50000.0f;
Color fondo = { 110, 100, 215, 255 };
Color textoPrincipal = RAYWHITE;
Color textoSecundario = DARKPURPLE;
Color sueloColor = Fade(DARKGREEN, 0.7f);

Game::Game(int screenWidth, int screenHeight)
	: physicsWorld(std::make_unique<PhysicsWorld>(GRAVITY))
	, renderer(std::make_unique<Renderer>(Color{ 110, 100, 215, 255 }))
{
	b2World& world = *physicsWorld->GetWorld();

	// Renderer
	Renderer renderer(fondo);

	Scenario scenario(world, screenWidth, screenHeight);
	groundBody = scenario.GetGroundBody();
	b2Vec2 groundPosition = groundBody->GetPosition();
	groundEntity = new RectangleEntity(groundPosition.x, groundPosition.y, screenWidth, 40.0f, sueloColor);

	// -----------------------------
	// Crear un c�rculo
	// -----------------------------
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
		float angleInRadians = impulseAngle * DEG2RAD;

		// Definir rotaci�n de la pr�xima caja
		if (IsKeyPressed(KEY_SPACE)) {
			b2Vec2 impulse(impulseStrength * cosf(angleInRadians), -impulseStrength * sinf(angleInRadians));
			circleBody->ApplyLinearImpulseToCenter(impulse, true);
		}

		if (IsKeyDown(KEY_LEFT)) {
			impulseAngle += 1.0f;
		}
		else if (IsKeyDown(KEY_RIGHT)) {
			impulseAngle -= 1.0f;
		}

		// Avanzar simulaci�n
		float deltaTime = GetFrameTime();
		physicsWorld->Update(deltaTime);

		renderer->Begin();

		// Suelo
		groundEntity->Update(groundBody, deltaTime, *renderer);

		// C�rculo
		circleEntity->Update(circleBody, deltaTime, *renderer);

		Vector2 lineStartPosition = {
			circleBody->GetPosition().x,
			circleBody->GetPosition().y
		};

		Vector2 lineEndPosition = {
			circleBody->GetPosition().x + 50.0f * cosf(angleInRadians),
			circleBody->GetPosition().y - 50.0f * sinf(angleInRadians)
		};

		DrawLineEx(lineStartPosition, lineEndPosition, 2.0f, GREEN);

		// Panel superior
		DrawRectangle(90, 40, 820, 70, Fade(BLACK, 0.18f));
		renderer->DrawCenteredText("Aplicacion de fuerzas en Box2D", 28, 60, textoPrincipal);

		// Pie
		renderer->DrawCenteredText("Flechas para cambiar direccion. Espacio para aplicar impulso", 20, 550, RAYWHITE);

		renderer->End();
	}
}