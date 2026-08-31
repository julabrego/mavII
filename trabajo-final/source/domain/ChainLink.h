#pragma once
#include "../core/Renderer.h"
#include "Entity.h"
#include <memory>
#include "RectangleEntity.h"
#include "../core/BodyData.h"
#include "../game/GameState.h"

const float LINK_DEFAULT_DENSITY = 1.0f;
const float LINK_DEFAULT_FRICTION = 1.0f;
const float LINK_DEFAULT_RESTITUTION = 0.0f;
const float LINK_DEFAULT_LINEAR_DAMPING = 0.0f;
const float LINK_DEFAULT_ANGULAR_DAMPING = 2.0f;
const float LINK_WIDTH = 15.0f;
const float LINK_HEIGHT = 15.0f;

class ChainLink :
	public Entity
{
public:
	ChainLink(b2World& world, float startX, float startY, float angle = 0.0f, float density = LINK_DEFAULT_DENSITY, float friction = LINK_DEFAULT_FRICTION, float restitution = LINK_DEFAULT_RESTITUTION, float linearDamping = LINK_DEFAULT_LINEAR_DAMPING, float angularDamping = LINK_DEFAULT_ANGULAR_DAMPING);
	~ChainLink();
	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;
	void Launch(float angleRad, float speed);
	b2Body* GetBody() const { return hitbox->GetBody(); }

private:
	std::unique_ptr<RectangleEntity> hitbox;
	BodyData bodyData;

	float density = LINK_DEFAULT_DENSITY;
	float friction = LINK_DEFAULT_FRICTION;
	float restitution = LINK_DEFAULT_RESTITUTION;
	float linearDamping = LINK_DEFAULT_LINEAR_DAMPING;
	float angularDamping = LINK_DEFAULT_ANGULAR_DAMPING;
};

