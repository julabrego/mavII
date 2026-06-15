#include "Renderer.h"
#include "../domain/CircleEntity.h"
#include "../domain/RectangleEntity.h"
#include "raylib.h"
#include "rlgl.h"

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
	rlPushMatrix();
		rlTranslatef(rectangle.position.x + rectangle.width / 2, rectangle.position.y + rectangle.height / 2, 0);
		rlRotatef(rectangle.angle, 0, 0, 1);
		DrawRectangleRec({ -rectangle.width / 2, -rectangle.height / 2, rectangle.width, rectangle.height }, rectangle.color);
		if (rectangle.borderThickness > 0.0f) {
			DrawRectangleLinesEx({ -rectangle.width / 2, -rectangle.height / 2, rectangle.width, rectangle.height }, rectangle.borderThickness, rectangle.borderColor);
		}
	rlPopMatrix();
}

void Renderer::DrawCenteredText(const char* text, int fontSize, int posY, Color color) {
	DrawText(text, GetScreenWidth() / 2 - MeasureText(text, fontSize) / 2, posY, fontSize, color);
}