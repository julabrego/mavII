#include "Enemy.h"
#include "../core/BodyData.h"

Enemy::Enemy(b2World& world, float startX, float startY) {
	hitbox = RectangleEntity::CreateDynamic(world, startX, startY, ENEMY_WIDTH, ENEMY_HEIGHT, 0.0f, RED, density, friction, bounciness);
	hitbox->GetBody()->SetFixedRotation(true);
	BodyData* enemyData = new BodyData({ BodyTag::Enemy, this });
	hitbox->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(enemyData);

}

Enemy::~Enemy() {
}

void Enemy::TakeDamage() {
	if (state != EnemyState::TakingDamage && state != EnemyState::Dead) {
		state = EnemyState::TakingDamage;
	}
}

void Enemy::Die() {
	if (state != EnemyState::Dead) {
		state = EnemyState::Dead;
	}
}

void Enemy::Update(float deltaTime) {
	if (state == EnemyState::TakingDamage) {
		life--;
		if(life <= 0) {
			Die();
		} else {
			state = EnemyState::Moving; 
		}
	}
}

void Enemy::Render(Renderer& renderer) {
	hitbox->Render(renderer);
}