#include "CircleEntity.h"
#include "../core/Renderer.h"
#include "../core/PhysicsConstants.h"
#include "box2d.h"

std::unique_ptr<CircleEntity> CircleEntity::CreateStatic(b2World& world, float x, float y, float radius, Color color, float borderThickness, Color borderColor)
{
	return std::unique_ptr<CircleEntity>(new CircleEntity(world, x, y, radius, color, b2_staticBody, 0.0f, 0.0f, 0.0f, false, borderThickness, borderColor));
}

std::unique_ptr<CircleEntity> CircleEntity::CreateDynamic(b2World& world, float x, float y, float radius, Color color, float density, float friction, float restitution, float borderThickness, Color borderColor)
{
	return std::unique_ptr<CircleEntity>(new CircleEntity(world, x, y, radius, color, b2_dynamicBody, density, friction, restitution, false, borderThickness, borderColor));
}

std::unique_ptr<CircleEntity> CircleEntity::CreateSensor(b2World& world, float x, float y, float radius, Color color, float borderThickness, Color borderColor)
{
	return std::unique_ptr<CircleEntity>(new CircleEntity(world, x, y, radius, color, b2_staticBody, 0.0f, 0.0f, 0.0f, true, borderThickness, borderColor));
}

CircleEntity::CircleEntity(b2World& world, float x, float y, float radius, Color color, b2BodyType type, float density, float friction, float restitution, bool isSensor, float borderThickness, Color borderColor)
	: position({ x, y }),
	radius(radius),
	color(color),
	borderThickness(borderThickness),
	borderColor(borderColor)
{
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(x * METERS_PER_PIXEL, y * METERS_PER_PIXEL);

	body = world.CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius * METERS_PER_PIXEL;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.isSensor = isSensor;

	body->CreateFixture(&fixtureDef);
}

void CircleEntity::Update(float deltaTime)
{
	this->position = {
		body->GetPosition().x * PIXELS_PER_METER,
		body->GetPosition().y * PIXELS_PER_METER
	};
	this->angle = body->GetAngle() * RAD2DEG;
}

void CircleEntity::Render(Renderer& renderer)
{
	renderer.Draw(*this);
}
