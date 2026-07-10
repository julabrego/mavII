#pragma once
#include "Entity.h"
#include <memory>
#include "RectangleEntity.h"
#include "../core/Renderer.h"

enum class PlayerAction {
	Left,
	Right,
	Jump
};

class Player : public Entity
{
public:
	Player(b2World& world, float startX = GetScreenWidth() / 2.0f, float startY = GetScreenHeight() / 2.0f);
	~Player();

	void SetAction(PlayerAction action, bool active);
	void IncrementGroundContacts();
	void DecrementGroundContacts();

	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

private:
	static constexpr float PLAYER_WIDTH = 30.0f;
	static constexpr float PLAYER_HEIGHT = 60.0f;
	static constexpr float SENSOR_HEIGHT = 12.0f;
	static constexpr Color SENSOR_DEBUG_COLOR = RED;

	struct ActionState {
		bool left = false;
		bool right = false;
		bool jump = false;
	} actionState;

	std::unique_ptr<RectangleEntity> hitbox;

	float moveSpeed = 5.0f;
	float jumpImpulse = 18.0f;
	float density = 1.0f;
	float bounciness = 0.0f;
	float friction = 0.5f;

	bool isGrounded = false;
	int groundContactCount = 0;

};