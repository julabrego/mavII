#include "Circle.h"
#include "../core/Renderer.h"

Circle::Circle(float x, float y, float radius, Color color, float borderThickness, Color borderColor)
	: position({ x, y }),
	radius(radius),
	color(color),
	borderThickness(borderThickness),
	borderColor(borderColor)
{
}

void Circle::Update(float deltaTime, Renderer& renderer)
{
	renderer.Draw(*this);
}

void Circle::SetPosition(float x, float y)
{
	position = { x, y };
}

