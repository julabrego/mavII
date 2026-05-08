#include "RectangleEntity.h"
#include "../core/Renderer.h"
#include "box2d.h"

RectangleEntity::RectangleEntity(float x, float y, float width, float height, Color color, float borderThickness, Color borderColor)
	: position({ x, y }),
	width(width),
	height(height),
	color(color),
	borderThickness(borderThickness),
	borderColor(borderColor)
{
}

void RectangleEntity::Update(b2Body* phisicBody, float deltaTime, Renderer& renderer)
{
	b2Vec2 center = phisicBody->GetPosition();
	this->position = {
		center.x - width / 2,
		center.y - height / 2
	};
	renderer.Draw(*this);
}