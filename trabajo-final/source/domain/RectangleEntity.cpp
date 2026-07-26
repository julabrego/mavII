#include "RectangleEntity.h"
#include "../core/Renderer.h"
#include "../core/PhysicsConstants.h"
#include "box2d.h"

std::unique_ptr<RectangleEntity> RectangleEntity::CreateStatic(b2World& world, float x, float y, float width, float height, float angle, Color color, float friction, float borderThickness, Color borderColor)
{
	return std::unique_ptr<RectangleEntity>(new RectangleEntity(world, x, y, width, height, angle, color, b2_staticBody, 0.0f, friction, 0.0f, borderThickness, borderColor, false));
}

std::unique_ptr<RectangleEntity> RectangleEntity::CreateDynamic(b2World& world, float x, float y, float width, float height, float angle, Color color, float density, float friction, float restitution, float borderThickness, Color borderColor)
{
	return std::unique_ptr<RectangleEntity>(new RectangleEntity(world, x, y, width, height, angle, color, b2_dynamicBody, density, friction, restitution, borderThickness, borderColor, false));
}

std::unique_ptr<RectangleEntity> RectangleEntity::CreateSensor(b2World& world, float x, float y, float width, float height, float angle, Color color, float borderThickness, Color borderColor)
{
	return std::unique_ptr<RectangleEntity>(new RectangleEntity(world, x, y, width, height, angle, color, b2_staticBody, 0.0f, 0.0f, 0.0f, borderThickness, borderColor, true));
}

RectangleEntity::RectangleEntity(b2World& world, float x, float y, float width, float height, float angle, Color color, b2BodyType type, float density, float friction, float restitution, float borderThickness, Color borderColor, bool isSensor)
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
	bodyDef.position.Set((x + width / 2) * METERS_PER_PIXEL, (y + height / 2) * METERS_PER_PIXEL);
	bodyDef.angle = angle * DEG2RAD;

	body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((width / 2) * METERS_PER_PIXEL, (height / 2) * METERS_PER_PIXEL);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.isSensor = isSensor;

	body->CreateFixture(&fixtureDef);
}

void RectangleEntity::Update(float deltaTime)
{
	b2Vec2 center = body->GetPosition();
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	this->position = {
		center.x * PIXELS_PER_METER - halfWidth,
		center.y * PIXELS_PER_METER - halfHeight
	};
	this->angle = body->GetAngle() * RAD2DEG;
}

void RectangleEntity::Render(Renderer& renderer)
{
	renderer.Draw(*this);
}
