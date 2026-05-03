#include "Renderer.h"

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

void Renderer::DrawCircle(Circle circle)
{
	DrawCircleV(circle.position, circle.radius, circle.color);
	DrawCircleLines(circle.position.x, circle.position.y, circle.radius, circle.borderColor);
}