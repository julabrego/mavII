#include "../core/Renderer.h"
#include "box2d.h"
#include "Stick.h"
#include "CircleEntity.h"
#include "RectangleEntity.h"

StickEntity::StickEntity(b2World& world, float x, float y, Color color)
	: position({ x, y }),
	color(color)
{
	/*b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	axisBody = world.CreateBody(&bodyDef);
	b2CircleShape shape;
	shape.m_radius = 25.0f;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	
	axisBody->CreateFixture(&fixtureDef);*/

	axis = std::make_unique<CircleEntity>(world, x, y, 18.0f, color, 1.0f, 0.5f, 0.3f, 2.0f);
	base = RectangleEntity::CreateDynamic(world, x - 10.0f, y - 15.0f, 100.0f, 30.0f, 0.0f, color, 1.0f, 0.0f, 0.0f);

	b2WeldJointDef weldJoint;
	weldJoint.Initialize(axis->GetBody(), base->GetBody(),
		b2Vec2({ axis->position.x, axis->position.y }));
	b2WeldJoint* weldObstaclesJoint = (b2WeldJoint*)world.CreateJoint(&weldJoint);

	b2RevoluteJointDef revoluteJoint;
	revoluteJoint.Initialize(axis->GetBody(), base->GetBody(),
		b2Vec2({ axis->position.x, axis->position.y }));
	b2RevoluteJoint* weldObstacle1RevoluteJoint = (b2RevoluteJoint*)world.CreateJoint(&revoluteJoint);
}

void StickEntity::Update(float deltaTime)
{
	this->position = {
		axis->position.x,
		axis->position.y
	};
}

void StickEntity::Render(Renderer& renderer)
{
	renderer.Draw(*this->axis);
	renderer.Draw(*this->base);
}