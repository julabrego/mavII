#include "CircleEntity.h"
#include "../core/Renderer.h"
#include "box2d.h"

CircleEntity::CircleEntity(float x, float y, float radius, Color color, float borderThickness, Color borderColor)
	: position({ x, y }),
	radius(radius),
	color(color),
	borderThickness(borderThickness),
	borderColor(borderColor)
{
}

void CircleEntity::Update(b2Body* phisicBody, float deltaTime, Renderer& renderer)
{
	this->position = {
		phisicBody->GetPosition().x,
		phisicBody->GetPosition().y
	};
	renderer.Draw(*this);
}

