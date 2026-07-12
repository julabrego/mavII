#pragma once
#include "Entity.h"
#include "../core/Renderer.h"
#include "RectangleEntity.h"
#include <box2d.h>
#include "../game/GameState.h"	
#include <raylib.h>

enum class EnemyState {
	Moving,
	TakingDamage,
	Dead
};

class Enemy : public Entity
{
public:
	Enemy(b2World& world, GameContext& gameContext, float startX = GetScreenWidth() / 2.0f, float startY = GetScreenHeight() / 2.0f);
	~Enemy();

	void TakeDamage();
	void Die();

	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;
	void DrawDebugSensors(Renderer& renderer);
	EnemyState GetState() const { return state; }
	b2Body* GetBody() const { return hitbox->GetBody(); }

private:
	EnemyState state = EnemyState::Moving;

	static constexpr float ENEMY_WIDTH = 30.0f;
	static constexpr float ENEMY_HEIGHT = 60.0f;

	std::unique_ptr<RectangleEntity> hitbox;
	b2World& world;
	float direction = 1.0f;
	float moveSpeed = 2.0f;
	float density = 1.0f;
	float bounciness = 0.0f;
	float friction = 0.5f;
	int life = 1;

	b2Fixture* rightEdgeSensor = nullptr;
	b2Fixture* leftEdgeSensor = nullptr;
	float wallCooldown = 0.0f;

	GameContext& context;

	bool facingRight = true;
	Texture2D enemyTexture = LoadTexture("../assets/enemy.png");

};
