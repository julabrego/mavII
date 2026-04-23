#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>

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
	b2Vec2 gravity(0.0f, 9.8f);
	b2World world(gravity);

	// -----------------------------
	// Suelo estático
	// -----------------------------
	b2BodyDef groundDef;
	groundDef.type = b2_staticBody;
	groundDef.position.Set(screenWidth / 2.0f, screenHeight - 40.0f);
	b2Body* groundBody = world.CreateBody(&groundDef);

	b2PolygonShape groundShape;
	groundShape.SetAsBox(screenWidth / 2.0f, 20.0f);
	groundBody->CreateFixture(&groundShape, 0.0f);

	std::vector<PhysicsBox> boxes;
	std::vector<PhysicsCircle> circles;


	while (!WindowShouldClose())
	{
		// Avanzar simulación
		world.Step(1.0f / 60.0f, 8, 3);

		BeginDrawing();
		ClearBackground(fondo);

		// Suelo visual
		DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

		if (IsKeyPressed(KEY_SPACE)) {
			TraceLog(LOG_INFO, "Creando nueva caja dinámica...");
			b2BodyDef boxDef;
			boxDef.type = b2_dynamicBody;
			boxDef.position.Set(GetScreenWidth() / 2.0f, 80.0f);

			b2Body* boxBody = world.CreateBody(&boxDef);

			b2PolygonShape boxShape;
			boxShape.SetAsBox(25.0f, 25.0f);

			b2FixtureDef boxFixture;
			boxFixture.shape = &boxShape;
			boxFixture.density = 1.0f;
			boxFixture.friction = 0.4f;
			boxFixture.restitution = 0.2f;

			boxBody->CreateFixture(&boxFixture);

			boxes.push_back({ boxBody, 100.0f, 50.0f, Fade(SKYBLUE, 0.95f) });
		}

		// Dibujar cajas
		for (const auto& box : boxes)
		{
			b2Vec2 pos = box.body->GetPosition();
			float angle = box.body->GetAngle() * RAD2DEG;

			Rectangle rect = {
				pos.x - box.width / 2.0f,
				pos.y - box.height / 2.0f,
				box.width,
				box.height
			};

			Vector2 origin = { 0, 0 };
			DrawRectanglePro(rect, origin, angle, box.color);
			DrawRectangleLinesEx(rect, 2, DARKBLUE);
		}

		// Panel superior
		DrawRectangle(90, 70, 820, 90, Fade(BLACK, 0.18f));
		DrawText("Bienvenidos a Modelos y Algoritmos para Videojuegos II", 120, 90, 28, textoPrincipal);
		DrawText("Raylib dibuja. Box2D simula.", 320, 125, 22, textoSecundario);

		// Pie
		DrawText("Primer contacto con simulacion fisica en 2D", 300, 540, 20, RAYWHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}