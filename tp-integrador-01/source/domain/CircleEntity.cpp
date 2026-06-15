#include "CircleEntity.h"
#include "../core/Renderer.h"
#include "../core/PhysicsConstants.h"
#include "box2d.h"

CircleEntity::CircleEntity(b2World& world, float x, float y, float radius, Color color, float density, float friction, float restitution, float borderThickness, Color borderColor, b2BodyType bodyType)
	: position({ x, y }),
	radius(radius),
	color(color),
	borderThickness(borderThickness),
	borderColor(borderColor)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * METERS_PER_PIXEL, y * METERS_PER_PIXEL);

	body = world.CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius * METERS_PER_PIXEL;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	body->CreateFixture(&fixtureDef);
}

void CircleEntity::Update(float deltaTime)
{
	this->position = {
		body->GetPosition().x * PIXELS_PER_METER,
		body->GetPosition().y * PIXELS_PER_METER
	};
}

void CircleEntity::Render(Renderer& renderer)
{
	renderer.Draw(*this);
}

