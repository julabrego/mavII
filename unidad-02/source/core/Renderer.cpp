#include "Renderer.h"
#include "../domain/Circle.h"
#include "raylib.h"

Renderer::Renderer(Color clearColor)
	: clearColor(std::make_unique<Color>(clearColor))
{
}
Renderer::~Renderer()
{
}

void Renderer::Begin()
{
	BeginDrawing();
	ClearBackground(*clearColor);
}

void Renderer::End()
{
	EndDrawing();
}

void Renderer::Draw(const Circle& circle)
{
	printf("Dibujando círculo en posición (%.2f, %.2f) con radio %.2f\n", circle.position.x, circle.position.y, circle.radius);
	DrawCircleV(circle.position, circle.radius, circle.color);
	if (circle.borderThickness > 0.0f) {
		DrawCircleLines(circle.position.x, circle.position.y, circle.radius, circle.borderColor);
	}
}