#include "../core/Renderer.h"
#include "box2d.h"
#include "Stick.h"
#include "CircleEntity.h"
#include "RectangleEntity.h"

StickEntity::StickEntity(b2World& world, float x, float y, Color color, bool isLeftStick = true)
	: position({ x, y }),
	color(color),
	isLeftStick(isLeftStick)
{
	axis = std::make_unique<CircleEntity>(world, x, y, 18.0f, color, 1.0f, 0.5f, 0.0f, 2.0f, BLACK, b2_staticBody);

	float baseX = isLeftStick ? (x - 10.0f) : (x - 90.0f);
	base = RectangleEntity::CreateDynamic(world, baseX, y - 15.0f, 100.0f, 30.0f, 0.0f, color, 0.1f, 0.0f, 0.0f);

	b2RevoluteJointDef revoluteJoint;
	revoluteJoint.Initialize(axis->GetBody(), base->GetBody(),
		b2Vec2({ axis->position.x, axis->position.y }));
	revoluteJoint.enableLimit = true;
	revoluteJoint.lowerAngle = -30.0f * DEG2RAD;
	revoluteJoint.upperAngle = 30.0f * DEG2RAD;
	revoluteJoint.enableMotor = true;
	revoluteJoint.motorSpeed = isLeftStick ? 20.0f : -20.0f;
	revoluteJoint.maxMotorTorque = 500000.0f;
	joint = (b2RevoluteJoint*)world.CreateJoint(&revoluteJoint);
}

void StickEntity::TriggerAction()
{
	joint->SetMotorSpeed(0.0f);
	base->GetBody()->SetAngularVelocity(isLeftStick ? -25.0f : 25.0f);
}

void StickEntity::Reset()
{
	base->GetBody()->SetAngularVelocity(isLeftStick ? 50.0f : -50.0f);
	joint->SetMotorSpeed(isLeftStick ? 20.0f : -20.0f);
}

void StickEntity::Update(float deltaTime)
{
	axis->Update(deltaTime);
	base->Update(deltaTime);
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