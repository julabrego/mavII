#include "Player.h"

Player::Player(b2World& world, float startX, float startY) {
	hitbox = RectangleEntity::CreateDynamic(world, startX, startY, 30.0f, 60.0f, 0.0f, YELLOW, density, friction, bounciness);
}

Player::~Player() {
}

void Player::SetAction(PlayerAction action, bool active) {
	switch (action) {
	case PlayerAction::Left:
		actionState.left = active;
		break;
	case PlayerAction::Right:
		actionState.right = active;
		break;
	case PlayerAction::Jump:
		actionState.jump = active;
		break;
	default:
		break;
	}
}

void Player::Update(float deltaTime) {
	hitbox->Update(deltaTime);
}

void Player::Render(Renderer& renderer) {
	hitbox->Render(renderer);
}
