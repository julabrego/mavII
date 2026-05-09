#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>
#include "main.h"

#include "core/PhysicsWorld.h"
#include "core/Renderer.h"
#include "domain/CircleEntity.h"
#include "domain/RectangleEntity.h"
#include "domain/Scenario.h"

float impulseAngle = 0.0f;
float impulseStrength = 50000.0f;

const float GRAVITY = 9.8f;

int main(void)
{
	const int screenWidth = 1000;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "MAVI II - Bienvenida");
	SetTargetFPS(60);

	Color fondo = { 110, 100, 215, 255 };
	Color textoPrincipal = RAYWHITE;
	Color textoSecundario = DARKPURPLE;
	Color sueloColor = Fade(DARKGREEN, 0.7f);

	// Renderer
	Renderer renderer(fondo);

	// Mundo f�sico
	PhysicsWorld physicsWorld(GRAVITY);
	b2World& world = *physicsWorld.GetWorld();

	Scenario scenario(world, screenWidth, screenHeight);
	b2Body* groundBody = scenario.GetGroundBody();
	b2Vec2 groundPosition = groundBody->GetPosition();

	std::unique_ptr<RectangleEntity> groundEntity;
	groundEntity = std::make_unique<RectangleEntity>(groundPosition.x, groundPosition.y, static_cast<float>(screenWidth), 40.0f, sueloColor);

	// -----------------------------
	// Crear un c�rculo
	// -----------------------------
	b2BodyDef circleDef;
	circleDef.type = b2_dynamicBody;
	circleDef.position.Set(GetScreenWidth() / 2, 50.0f);

	b2Body* circleBody = world.CreateBody(&circleDef);

	b2CircleShape circleShape;
	circleShape.m_radius = 30.0f;

	b2FixtureDef circleFixture;
	circleFixture.shape = &circleShape;
	circleFixture.density = 0.5f;
	circleFixture.friction = 0.5f;
	circleFixture.restitution = 0.5f;

	circleBody->CreateFixture(&circleFixture);

	CircleEntity circle(circleBody->GetPosition().x, circleBody->GetPosition().y, circleShape.m_radius, DARKBLUE, 2.0f, BLACK);
	RectangleEntity ground(groundBody->GetPosition().x, groundBody->GetPosition().y, screenWidth, 40.0f, sueloColor);

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
		physicsWorld.Update(deltaTime);

		renderer.Begin();

		// Suelo
		groundEntity->Update(groundBody, deltaTime, renderer);

		// C�rculo
		circle.Update(circleBody, deltaTime, renderer);

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
		DrawCenteredText("Aplicacion de fuerzas en Box2D", 28, 60, textoPrincipal);

		// Pie
		DrawCenteredText("Flechas para cambiar direccion. Espacio para aplicar impulso", 20, 550, RAYWHITE);

		renderer.End();
	}

	CloseWindow();
	return 0;
}

void DrawCenteredText(const char* text, int fontSize, int posY, Color color) {
	DrawText(text, GetScreenWidth() / 2 - MeasureText(text, fontSize) / 2, posY, fontSize, color);
}