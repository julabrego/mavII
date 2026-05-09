#include "RectangleEntity.h"
#include "../core/Renderer.h"
#include "box2d.h"

RectangleEntity::RectangleEntity(b2World& world, float x, float y, float width, float height, Color color, float borderThickness, Color borderColor)
	: position({ x, y }),
	width(width),
	height(height),
	color(color),
	borderThickness(borderThickness),
	borderColor(borderColor)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x + width / 2, y + height / 2);

	body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(width / 2, height / 2);

	body->CreateFixture(&shape, 0.0f);
}

void RectangleEntity::Update(float deltaTime, Renderer& renderer)
{
	b2Vec2 center = body->GetPosition();
	this->position = {
		center.x - width / 2,
		center.y - height / 2
	};
}

void RectangleEntity::Render(Renderer& renderer)
{
	renderer.Draw(*this);
}