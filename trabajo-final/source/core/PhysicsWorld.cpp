#include "PhysicsWorld.h"

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
	world->Step(delta, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

void PhysicsWorld::DestroyBody(b2Body* body)
{
	world->DestroyBody(body);
}