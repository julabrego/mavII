#pragma once
#include "../core/Renderer.h"
#include "Entity.h"
#include <memory>
#include "CircleEntity.h"
#include "../core/BodyData.h"
#include "../game/GameState.h"

const float WRECKING_BALL_DEFAULT_RADIUS = 10.0f;
const float WRECKING_BALL_DEFAULT_DENSITY = 10.0f;
const float WRECKING_BALL_DEFAULT_FRICTION = 1.0f;
const float WRECKING_BALL_DEFAULT_RESTITUTION = 0.5f;
const float WRECKING_BALL_DEFAULT_LINEAR_DAMPING = 0.0f;
const float WRECKING_BALL_DEFAULT_ANGULAR_DAMPING = 2.0f;

class WreckingBall :
    public Entity
{
public:
    WreckingBall(b2World& world, GameContext& context, float startX, float startY, float radius = WRECKING_BALL_DEFAULT_RADIUS, float density = WRECKING_BALL_DEFAULT_DENSITY, float friction = WRECKING_BALL_DEFAULT_FRICTION, float restitution = WRECKING_BALL_DEFAULT_RESTITUTION, float linearDamping = WRECKING_BALL_DEFAULT_LINEAR_DAMPING, float angularDamping = WRECKING_BALL_DEFAULT_ANGULAR_DAMPING);
    ~WreckingBall();
    void Update(float deltaTime) override;
    void Render(Renderer& renderer) override;
	void Launch(float angleRad, float speed);
	b2Body* GetBody() const { return hitbox->GetBody(); }

private:
	std::unique_ptr<CircleEntity> hitbox;
	BodyData bodyData;

	float radius = WRECKING_BALL_DEFAULT_RADIUS;
    float density = WRECKING_BALL_DEFAULT_DENSITY;
	float friction = WRECKING_BALL_DEFAULT_FRICTION;
    float restitution = WRECKING_BALL_DEFAULT_RESTITUTION;
	float linearDamping = WRECKING_BALL_DEFAULT_LINEAR_DAMPING;
	float angularDamping = WRECKING_BALL_DEFAULT_ANGULAR_DAMPING;

	GameContext& context;

	Texture2D texture = LoadTexture("assets/wreckingball.png");
};

