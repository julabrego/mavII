#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <box2d.h>
#include <memory>

class PhysicsWorld
{
public:
	PhysicsWorld(float gravityY);
	~PhysicsWorld();

	void Update(float delta);

	b2World* GetWorld() { return world.get(); }

private:
	std::unique_ptr<b2World> world;

	const int VELOCITY_ITERATIONS = 6;
	const int POSITION_ITERATIONS = 2;
};

#endif