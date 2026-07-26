#pragma once
#include "../core/Renderer.h"
#include "Entity.h"
#include <memory>
#include "CircleEntity.h"
#include "../core/BodyData.h"
#include "../game/GameState.h"

const float DEFAULT_RADIUS = 10.0f;
const float DEFAULT_DENSITY = 4.0f;
const float DEFAULT_FRICTION = 1.0f;
const float DEFAULT_RESTITUTION = 0.5f;
const float DEFAULT_LINEAR_DAMPING = 0.0f;
const float DEFAULT_ANGULAR_DAMPING = 2.0f;

class WreckingBall :
    public Entity
{
public:
    WreckingBall(b2World& world, float startX, float startY, float radius = DEFAULT_RADIUS, float density = DEFAULT_DENSITY, float friction = DEFAULT_FRICTION, float restitution = DEFAULT_RESTITUTION, float linearDamping = DEFAULT_LINEAR_DAMPING, float angularDamping = DEFAULT_ANGULAR_DAMPING);
    ~WreckingBall();
    void Update(float deltaTime) override;
    void Render(Renderer& renderer) override;
	void Launch(float angleRad, float speed);
	b2Body* GetBody() const { return hitbox->GetBody(); }

private:
	std::unique_ptr<CircleEntity> hitbox;
	BodyData bodyData;

	float radius = DEFAULT_RADIUS;
    float density = DEFAULT_DENSITY;
	float friction = DEFAULT_FRICTION;
    float restitution = DEFAULT_RESTITUTION;
	float linearDamping = DEFAULT_LINEAR_DAMPING;
	float angularDamping = DEFAULT_ANGULAR_DAMPING;
};

