#include "Player.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"
#include <algorithm>

Player::Player(b2World& world, GameContext& gameContext, float startX, float startY) : context(gameContext) {
	hitbox = RectangleEntity::CreateDynamic(world, startX, startY, PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f, Fade(YELLOW, 0.5f), density, friction, bounciness);
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
	UnloadTexture(playerTexture);
}

void Player::TakeDamage() {
	if (state != PlayerState::TakingDamage && state != PlayerState::Dead) {
		printf("Player has taken damage.\n");
		state = PlayerState::TakingDamage;
	}
}

void Player::Die() {
	if (state != PlayerState::Dead) {
		printf("Player has died.\n");
		state = PlayerState::Dead;
	}
}

void Player::IncrementGroundContacts() {
	groundContactCount++;
}

void Player::DecrementGroundContacts() {
	groundContactCount = std::max(0, groundContactCount - 1);
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

void Player::Bounce() {
	hitbox->GetBody()->SetLinearVelocity(b2Vec2(hitbox->GetBody()->GetLinearVelocity().x, 0.0f));
	hitbox->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -jumpImpulse / 2.0f), true);
}

void Player::TeleportTo(float x, float y) {
	hitbox->GetBody()->SetTransform(b2Vec2(x, y), 0.0f);
	hitbox->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
}

void Player::Update(float deltaTime) {
	if (context.state != GameState::Playing) {
		b2Body* body = hitbox->GetBody();
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		body->SetGravityScale(0.0f);
		return;
	}

	isGrounded = groundContactCount > 0 && hitbox->GetBody()->GetLinearVelocity().y > -2.0f;

	if (state == PlayerState::TakingDamage) {
		life--;
		if (life <= 0) {
			Die();
		}
		else {
			state = PlayerState::Idle;
		}
	}

	if (actionState.left) {
		hitbox->GetBody()->SetLinearVelocity(b2Vec2(-moveSpeed, hitbox->GetBody()->GetLinearVelocity().y));
		facingRight = false;
	}
	else if (actionState.right) {
		hitbox->GetBody()->SetLinearVelocity(b2Vec2(moveSpeed, hitbox->GetBody()->GetLinearVelocity().y));
		facingRight = true;
	}
	else {
		hitbox->GetBody()->SetLinearVelocity(b2Vec2(0.0f, hitbox->GetBody()->GetLinearVelocity().y));
	}

	if (actionState.jump && isGrounded) {
		hitbox->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -jumpImpulse), true);
		actionState.jump = false; // Prevent continuous jumping
	}

	if (state != PlayerState::TakingDamage && state != PlayerState::Dead) {
		if (isGrounded) {
			if (hitbox->GetBody()->GetLinearVelocity().x != 0.0f) {
				state = PlayerState::Walking;
			}
			else {
				state = PlayerState::Idle;
			}
		}
		else {
			if (hitbox->GetBody()->GetLinearVelocity().y < 0.0f) {
				state = PlayerState::Jumping;
			}
			else {
				state = PlayerState::Falling;
			}
		}
	}

	hitbox->Update(deltaTime);
}

void Player::Render(Renderer& renderer) {
	float drawWidth = facingRight ? (float)playerTexture.width : -(float)playerTexture.width;
	Rectangle src = { 0.0f, 0.0f, drawWidth, (float)playerTexture.height };
	Rectangle dst = { hitbox->position.x, hitbox->position.y + 3, (float)playerTexture.width, (float)playerTexture.height };
	renderer.DrawSprite(playerTexture, src, dst, WHITE);

	if (context.debugMode) {
		hitbox->Render(renderer);
		DrawDebugSensors(renderer);
	}
}

void Player::DrawDebugSensors(Renderer& renderer) {
	b2Vec2 pos = hitbox->GetBody()->GetPosition();
	float x = pos.x * PIXELS_PER_METER - PLAYER_WIDTH / 2.0f;
	float y = pos.y * PIXELS_PER_METER + PLAYER_HEIGHT / 2.0f - SENSOR_HEIGHT / 2.0f;
	renderer.DrawRect(x, y, PLAYER_WIDTH, SENSOR_HEIGHT, Fade(RED, 0.5f));
}