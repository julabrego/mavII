#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <box2d.h>
#include <memory>
#include "ContactListener.h"

class PhysicsWorld
{
public:
	PhysicsWorld(float gravityY);
	~PhysicsWorld();

	void Update(float delta);
	void DestroyBody(b2Body* body);

	b2World* GetWorld() { return world.get(); }

	ContactListener& GetContactListener() { return contactListener; }

private:
	std::unique_ptr<b2World> world;
	ContactListener contactListener;

	const int VELOCITY_ITERATIONS = 10;
	const int POSITION_ITERATIONS = 8;

	const float FIXED_TIME_STEP = 1.0f / 60.0f;
	const float MAX_FRAME_TIME = 0.25f;
	float accumulator = 0.0f;
};

#endif