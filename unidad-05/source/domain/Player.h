#pragma once
#include "Entity.h"
#include <memory>
#include "RectangleEntity.h"
#include "../core/Renderer.h"
#include "../game/GameState.h"

enum class PlayerAction {
	Left,
	Right,
	Jump
};

enum class PlayerState {
	Idle,
	Walking,
	Jumping,
	Falling,
	TakingDamage,
	Dead
};

class Player : public Entity
{
public:
	Player(b2World& world, GameContext& gameContext, float startX = GetScreenWidth() / 2.0f, float startY = GetScreenHeight() / 2.0f);
	~Player();

	void SetAction(PlayerAction action, bool active);
	void Bounce();
	void IncrementGroundContacts();
	void DecrementGroundContacts();
	void TakeDamage();
	void Die();

	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

	PlayerState GetState() const { return state; }
	b2Body* GetBody() const { return hitbox->GetBody(); }

private:
	void DrawDebugSensors(Renderer& renderer); 

	static constexpr float PLAYER_WIDTH = 30.0f;
	static constexpr float PLAYER_HEIGHT = 60.0f;
	static constexpr float SENSOR_HEIGHT = 12.0f;
	PlayerState state = PlayerState::Idle;

	struct ActionState {
		bool left = false;
		bool right = false;
		bool jump = false;
	} actionState;

	GameContext& context;

	std::unique_ptr<RectangleEntity> hitbox;

	float moveSpeed = 5.0f;
	float jumpImpulse = 18.0f;
	float density = 1.0f;
	float bounciness = 0.0f;
	float friction = 0.5f;

	bool isGrounded = false;
	int groundContactCount = 0;

	int life = 1;

	bool facingRight = true;
	Texture2D playerTexture = LoadTexture("../assets/mayro.png");

};