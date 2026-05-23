#include "RectangleEntity.h"
#include "../core/Renderer.h"
#include "box2d.h"

std::unique_ptr<RectangleEntity> RectangleEntity::CreateStatic(b2World& world, float x, float y, float width, float height, float angle, Color color, float borderThickness, Color borderColor)
{
	return std::unique_ptr<RectangleEntity>(new RectangleEntity(world, x, y, width, height, angle, color, b2_staticBody, 0.0f, 0.0f, 0.0f, borderThickness, borderColor));
}

std::unique_ptr<RectangleEntity> RectangleEntity::CreateDynamic(b2World& world, float x, float y, float width, float height, float angle, Color color, float density, float friction, float restitution, float borderThickness, Color borderColor)
{
    return std::unique_ptr<RectangleEntity>(new RectangleEntity(world, x, y, width, height, angle, color, b2_dynamicBody, density, friction, restitution, borderThickness, borderColor));
}

RectangleEntity::RectangleEntity(b2World& world, float x, float y, float width, float height, float angle, Color color, b2BodyType type, float density, float friction, float restitution, float borderThickness, Color borderColor)
	: position({ x, y }),
	width(width),
	height(height),
	angle(angle),
	color(color),
	borderThickness(borderThickness),
	borderColor(borderColor)
{
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(x + width / 2, y + height / 2);
	bodyDef.angle = angle * DEG2RAD;

	body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(width / 2, height / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	body->CreateFixture(&fixtureDef);
}

void RectangleEntity::Update(float deltaTime)
{
	b2Vec2 center = body->GetPosition();
	this->position = {
		center.x - width / 2,
		center.y - height / 2
	};
	this->angle = body->GetAngle() * RAD2DEG;
}

void RectangleEntity::Render(Renderer& renderer)
{
	renderer.Draw(*this);
}
