#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>
#include "main.h"

#include "core/PhysicsWorld.h"
#include "core/Renderer.h"

struct PhysicsBox
{
	b2Body* body;
	float width;
	float height;
	Color color;
};

struct PhysicsCircle
{
	b2Body* body;
	float radius;
	Color color;
};

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

	// Mundo físico
	PhysicsWorld physicsWorld(GRAVITY);
	b2World& world = *physicsWorld.GetWorld();

	// Renderer
	Renderer renderer(fondo);


	// -----------------------------
	// Suelo estático
	// -----------------------------
	b2BodyDef topWallDef;
	topWallDef.type = b2_staticBody;
	topWallDef.position.Set(0, -20.0f);
	b2Body* topWallBody = world.CreateBody(&topWallDef);

	b2PolygonShape topWallShape;
	topWallShape.SetAsBox(screenWidth, 20.0f);
	topWallBody->CreateFixture(&topWallShape, 0.0f);

	b2BodyDef groundDef;
	groundDef.type = b2_staticBody;
	groundDef.position.Set(screenWidth / 2.0f, screenHeight - 40.0f);
	b2Body* groundBody = world.CreateBody(&groundDef);

	b2PolygonShape groundShape;
	groundShape.SetAsBox(screenWidth / 2.0f, 20.0f);
	groundBody->CreateFixture(&groundShape, 0.0f);

	b2BodyDef leftWallDef;
	leftWallDef.type = b2_staticBody;
	leftWallDef.position.Set(-20.0f, 0);
	b2Body* leftWallBody = world.CreateBody(&leftWallDef);

	b2PolygonShape leftWallShape;
	leftWallShape.SetAsBox(20.0f, screenHeight);
	leftWallBody->CreateFixture(&leftWallShape, 0.0f);

	b2BodyDef rightWallDef;
	rightWallDef.type = b2_staticBody;
	rightWallDef.position.Set(screenWidth + 20.0f, 0);
	b2Body* rightWallBody = world.CreateBody(&rightWallDef);

	b2PolygonShape rightWallShape;
	rightWallShape.SetAsBox(20.0f, screenHeight);
	rightWallBody->CreateFixture(&rightWallShape, 0.0f);


	std::vector<PhysicsBox> boxes;
	std::vector<PhysicsCircle> circles;

	// -----------------------------
	// Crear un círculo
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

	while (!WindowShouldClose())
	{
		float angleInRadians = impulseAngle * DEG2RAD;

		// Definir rotación de la próxima caja
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

		// Avanzar simulación
		float deltaTime = GetFrameTime();
		physicsWorld.Update(deltaTime);

		renderer.Begin();

		// Suelo visual
		DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

		DrawCircle(circleBody->GetPosition().x, circleBody->GetPosition().y, circleShape.m_radius, BLUE);
		DrawCircleLines(circleBody->GetPosition().x, circleBody->GetPosition().y, circleShape.m_radius, BLACK);

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