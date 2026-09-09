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
	Pull
};

enum class PlayerCannonState {
	Aiming,
	Shooting,
	Pulling
};

class PlayerCannon : public Entity
{
public:
	PlayerCannon(b2World& world, GameContext& gameContext, float startX = GetScreenWidth() / 2.0f, float startY = GetScreenHeight() / 2.0f);
	~PlayerCannon();

	void SetAction(PlayerCannonAction action, bool active);
	bool GetShootRequested() const { return shootRequested; }
	bool GetPullRequested() const { return pullRequested; }

	void TeleportTo(float x, float y);
	
	void Die();
	bool IsDead() const { return isDead; }
	void SetCannonState(PlayerCannonState newState);
	void SetShowBall(bool show) { showBall = show; }

	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

	PlayerCannonState GetState() const { return state; }
	b2Body* GetBody() const { return hitbox->GetBody(); }
	b2Vec2 GetPosition() const { return hitbox->GetBody()->GetPosition(); }

private:
	void HandleMovement(float deltaTime);
	void HandleRotation(float deltaTime);
	
	static constexpr float PLAYER_RADIUS = 30.0f;
	static constexpr float TOP_OFFSET = 40.0f;
	static constexpr float BOTTOM_OFFSET = 100.0f;
	static constexpr float MAX_ROTATION_ANGLE = 70.0f * DEG2RAD;
	static constexpr float MIN_ROTATION_ANGLE = -70.0f * DEG2RAD;

	PlayerCannonState state = PlayerCannonState::Aiming;

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
	bool pullRequested = false;

	float rotationSpeed = 5.0f;
	float turretMotorTorque = 200.0f;
	float velocityY = 0.0f;
	float moveAcceleration = 80.0f;
	float moveDamping = 0.85f;
	float maxMoveSpeed = 4.0f;

	Texture2D armBaseTexture = LoadTexture("assets/arm.png");
	Texture2D aimTexture = LoadTexture("assets/closedHandWithBall.png");
	Texture2D closedHandTexture = LoadTexture("assets/closedHand.png");
	Texture2D shootTexture = LoadTexture("assets/shootingHand.png");
	Texture2D pullTexture = LoadTexture("assets/pullingHand.png");
	Texture2D ballTexture = LoadTexture("assets/wreckingball.png");
	bool showBall = true;
	bool isDead = false;
	BodyData bodyData;
};