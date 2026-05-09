#include "Renderer.h"
#include "../domain/CircleEntity.h"
#include "../domain/RectangleEntity.h"
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

void Renderer::Draw(const CircleEntity& circle)
{
	DrawCircleV(circle.position, circle.radius, circle.color);
	if (circle.borderThickness > 0.0f) {
		DrawCircleLines(circle.position.x, circle.position.y, circle.radius, circle.borderColor);
	}
}

void Renderer::Draw(const RectangleEntity& rectangle)
{
	DrawRectangleV(rectangle.position, { rectangle.width, rectangle.height }, rectangle.color);
	if (rectangle.borderThickness > 0.0f) {
		DrawRectangleLinesEx({ rectangle.position.x, rectangle.position.y, rectangle.width, rectangle.height }, rectangle.borderThickness, rectangle.borderColor);
	}
}