#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>

#include "rlgl.h"
#include "main.h"

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

	float nextBoxRotation = 0.0f;

	while (!WindowShouldClose())
	{
		// Avanzar simulación
		world.Step(1.0f / 60.0f, 8, 3);

		BeginDrawing();
		ClearBackground(fondo);

		// Suelo visual
		DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

		// Crear nueva caja
		if (IsKeyPressed(KEY_SPACE)) {
			b2BodyDef boxDef;
			boxDef.type = b2_dynamicBody;
			boxDef.position.Set(GetScreenWidth() / 2.0f, 150.0f);
			boxDef.angle = nextBoxRotation * DEG2RAD;

			b2Body* boxBody = world.CreateBody(&boxDef);

			b2PolygonShape boxShape;
			boxShape.SetAsBox(50.0f, 25.0f);

			b2FixtureDef boxFixture;
			boxFixture.shape = &boxShape;
			boxFixture.density = 1.0f;
			boxFixture.friction = 0.4f;
			boxFixture.restitution = 0.2f;

			boxBody->CreateFixture(&boxFixture);

			boxes.push_back({ boxBody, 100.0f, 50.0f, Fade(SKYBLUE, 0.95f) });
		}
		// Definir rotación de la próxima caja
		else if (IsKeyDown(KEY_LEFT) && nextBoxRotation > -180.0f) {
			nextBoxRotation -= 1.0f;
		}
		else if (IsKeyDown(KEY_RIGHT) && nextBoxRotation < 180.0f) {
			nextBoxRotation += 1.0f;
		}

		// Dibujar cajas
		for (const auto& box : boxes)
		{
			b2Vec2 pos = box.body->GetPosition();
			float angle = box.body->GetAngle() * RAD2DEG;

			Rectangle rec = { -box.width / 2.0f, -box.height / 2.0f, box.width, box.height };

			rlPushMatrix();
			rlTranslatef(pos.x, pos.y, 0);
			rlRotatef(angle, 0, 0, 1);
			DrawRectangleRec(rec, box.color);
			DrawRectangleLinesEx(rec, 2, DARKBLUE);
			rlPopMatrix();
		}

		// Panel superior
		DrawRectangle(90, 10, 820, 110, Fade(BLACK, 0.18f));

		DrawCenteredText("Bienvenidos a Modelos y Algoritmos para Videojuegos II", 28, 20, textoPrincipal);
		DrawCenteredText("Raylib dibuja. Box2D simula.", 22, 65, textoSecundario);
		DrawCenteredText(TextFormat("<- %.2f grados ->", nextBoxRotation), 18, 95, textoPrincipal);
		
		// Pie
		DrawCenteredText("Espacio: crear nuevas cajas", 20, 550, RAYWHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void DrawCenteredText(const char* text, int fontSize, int posY, Color color) {
	DrawText(text, GetScreenWidth() / 2 - MeasureText(text, fontSize) / 2, posY, fontSize, color);
}