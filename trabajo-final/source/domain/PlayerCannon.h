#pragma once
#include "Entity.h"
#include <memory>
#include "CircleEntity.h"
#include "../core/Renderer.h"
#include "../core/BodyData.h"
#include "../game/GameState.h"

enum class PlayerCannonAction {
	RotateLeft,
	RotateRight,
	MoveDown,
	MoveUp,
	Shoot,
	Push,
	Pull
};

enum class PlayerCannonState {
	Idle,
	Rotating,
	Moving,
	Shooting,
	Pushing,
	Pulling,
	Dead
};

class PlayerCannon : public Entity
{
public:
	PlayerCannon(b2World& world, GameContext& gameContext, float startX = GetScreenWidth() / 2.0f, float startY = GetScreenHeight() / 2.0f);
	~PlayerCannon();

	void SetAction(PlayerCannonAction action, bool active);
	bool ConsumeShoot();
	
	void TeleportTo(float x, float y);
	
	void Die();

	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

	PlayerCannonState GetState() const { return state; }
	b2Body* GetBody() const { return hitbox->GetBody(); }

private:
	void HandleMovement(float deltaTime);
	void HandleRotation(float deltaTime);
	
	static constexpr float PLAYER_RADIUS = 30.0f;
	static constexpr float TOP_OFFSET = 40.0f;
	static constexpr float BOTTOM_OFFSET = 100.0f;
	static constexpr float MAX_ROTATION_ANGLE = 70.0f * DEG2RAD;
	static constexpr float MIN_ROTATION_ANGLE = -70.0f * DEG2RAD;

	PlayerCannonState state = PlayerCannonState::Idle;

	struct ActionState {
		bool rotateLeft = false;
		bool rotateRight = false;
		bool moveDown = false;
		bool moveUp = false;
		bool shoot = false;
		bool push = false;
		bool pull = false;
	} actionState;

	GameContext& context;

	std::unique_ptr<CircleEntity> hitbox;
	b2Body* baseBody = nullptr;
	b2RevoluteJoint* turretJoint = nullptr;

	bool shootRequested = false;

	float rotationSpeed = 5.0f;
	float turretMotorTorque = 30.0f;
	float velocityY = 0.0f;
	float moveAcceleration = 80.0f;
	float moveDamping = 0.85f;
	float maxMoveSpeed = 4.0f;
	
	Texture2D cannonBaseTexture = LoadTexture("../assets/cannonBase.png");
	Texture2D cannonTopTexture = LoadTexture("../assets/cannonTop.png");
	BodyData bodyData;
};