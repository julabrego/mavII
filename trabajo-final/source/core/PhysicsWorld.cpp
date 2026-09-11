#include "PhysicsWorld.h"
#include "PhysicsConstants.h"
#include <cmath>

PhysicsWorld::PhysicsWorld(float gravityY)
	: world(std::make_unique<b2World>(b2Vec2(0.0f, gravityY)))
{
	b2Vec2 gravity(0.0f, gravityY);
	world->SetContactListener(&contactListener);
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::Update(float delta)
{
	accumulator += fminf(delta, MAX_FRAME_TIME);

	while (accumulator >= FIXED_TIME_STEP)
	{
		world->Step(FIXED_TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
		accumulator -= FIXED_TIME_STEP;
	}
}

void PhysicsWorld::DestroyBody(b2Body* body)
{
	world->DestroyBody(body);
}

bool PhysicsWorld::IsSettled() const
{
	for (const b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext()) {
		if (!body->IsAwake()) {
			continue;
		}

		b2Vec2 velocity = body->GetLinearVelocity();
		if (velocity.LengthSquared() > SETTLE_SPEED_THRESHOLD * SETTLE_SPEED_THRESHOLD) {
			return false;
		}

		if (fabsf(body->GetAngularVelocity()) > SETTLE_ANGULAR_THRESHOLD) {
			return false;
		}
	}

	return true;
}