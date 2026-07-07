#include "Player.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"

Player::Player(b2World& world, float startX, float startY) {
	hitbox = RectangleEntity::CreateDynamic(world, startX, startY, PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f, YELLOW, density, friction, bounciness);
	hitbox->GetBody()->SetFixedRotation(true);
	BodyData* playerData = new BodyData({ BodyTag::Player, this });
	hitbox->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerData);

	b2PolygonShape sensorShape;
	sensorShape.SetAsBox((PLAYER_WIDTH / 2.0f) * METERS_PER_PIXEL, (SENSOR_HEIGHT / 2.0f) * METERS_PER_PIXEL,
		b2Vec2(0.0f, (PLAYER_HEIGHT / 2.0f) * METERS_PER_PIXEL), 0.0f);

	b2FixtureDef sensorFixtureDef;
	sensorFixtureDef.shape = &sensorShape;
	sensorFixtureDef.isSensor = true;

	hitbox->GetBody()->CreateFixture(&sensorFixtureDef);
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
	if(actionState.left) {
		hitbox->GetBody()->SetLinearVelocity(b2Vec2(-moveSpeed, hitbox->GetBody()->GetLinearVelocity().y));
	}
	else if(actionState.right) {
		hitbox->GetBody()->SetLinearVelocity(b2Vec2(moveSpeed, hitbox->GetBody()->GetLinearVelocity().y));
	}
	else {
		hitbox->GetBody()->SetLinearVelocity(b2Vec2(0.0f, hitbox->GetBody()->GetLinearVelocity().y));
	}

	if(actionState.jump && isGrounded) {
		hitbox->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -jumpImpulse), true);
		actionState.jump = false; // Prevent continuous jumping
	}

	hitbox->Update(deltaTime);
}

void Player::Render(Renderer& renderer) {
	hitbox->Render(renderer);

	// DEBUG: draw sensor
	b2Vec2 pos = hitbox->GetBody()->GetPosition();
	float x = pos.x * PIXELS_PER_METER - PLAYER_WIDTH / 2.0f;
	float y = pos.y * PIXELS_PER_METER + PLAYER_HEIGHT / 2.0f - SENSOR_HEIGHT / 2.0f;
	DrawRectangle(x, y, PLAYER_WIDTH, SENSOR_HEIGHT, SENSOR_DEBUG_COLOR);
}
