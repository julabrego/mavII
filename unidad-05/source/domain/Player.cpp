#include "Player.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"

Player::Player(b2World& world, float startX, float startY) {
	float width = 30.0f;
	float height = 60.0f;

	hitbox = RectangleEntity::CreateDynamic(world, startX, startY, width, height, 0.0f, YELLOW, density, friction, bounciness);
	BodyData* playerData = new BodyData({ BodyTag::Player });
	hitbox->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerData);

	b2PolygonShape sensorShape;
	sensorShape.SetAsBox(0.5f * METERS_PER_PIXEL, 2.0f * METERS_PER_PIXEL);

	b2FixtureDef sensorFixtureDef;
	sensorFixtureDef.shape = &sensorShape;
	sensorFixtureDef.isSensor = true;

	b2Fixture* sensorFixture = hitbox->GetBody()->CreateFixture(&sensorFixtureDef);
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

	// DEBUG
	b2Body* body = hitbox->GetBody();
	b2Vec2 pos = body->GetPosition();
	float sensorWidth = 30.0f;
	float sensorHeight = 5.0f;
	float x = pos.x * PIXELS_PER_METER - sensorWidth / 2.0f;
	float y = pos.y * PIXELS_PER_METER + hitbox->height / 2.0f - sensorHeight / 2.0f; // Position the sensor below the player
	DrawRectangle(x, y, sensorWidth, sensorHeight, RED);
}
