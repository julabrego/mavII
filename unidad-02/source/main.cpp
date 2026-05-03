#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>
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
	circleFixture.density = 1.0f;
	circleFixture.friction = 0.3f;
	circleFixture.restitution = 0.5f;

	circleBody->CreateFixture(&circleFixture);

    while (!WindowShouldClose())
    {
        // Avanzar simulación
        world.Step(1.0f / 60.0f, 8, 3);

        BeginDrawing();
        ClearBackground(fondo);

        // Suelo visual
        DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

		DrawCircle(circleBody->GetPosition().x, circleBody->GetPosition().y, circleShape.m_radius, RED);
		DrawCircleLines(circleBody->GetPosition().x, circleBody->GetPosition().y, circleShape.m_radius, BLACK);

        // Panel superior
        DrawRectangle(90, 40, 820, 70, Fade(BLACK, 0.18f));
        DrawCenteredText("Aplicacion de fuerzas en Box2D", 28, 60, textoPrincipal);

        // Pie
        DrawCenteredText("Flechas para cambiar direccion. Espacio para aplicar impulso", 20, 550, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawCenteredText(const char* text, int fontSize, int posY, Color color) {
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, fontSize) / 2, posY, fontSize, color);
}