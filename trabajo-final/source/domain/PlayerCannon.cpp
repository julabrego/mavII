#include "PlayerCannon.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"
#include <algorithm>
#include <cmath>

PlayerCannon::PlayerCannon(b2World& world, GameContext& gameContext, float startX, float startY) : context(gameContext) {
	hitbox = CircleEntity::CreateDynamic(world, startX, startY, PLAYER_RADIUS, Fade(YELLOW, 0.5f), 1.0f, 0.3f, 0.0f);

	bodyData = { BodyTag::Player, this };
	hitbox->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(&bodyData);
	hitbox->GetBody()->SetType(b2_kinematicBody);

	b2PolygonShape sensorShape;
	sensorShape.SetAsBox((PLAYER_WIDTH / 2.0f) * METERS_PER_PIXEL, (SENSOR_HEIGHT / 2.0f) * METERS_PER_PIXEL,
		b2Vec2(0.0f, (PLAYER_HEIGHT / 2.0f) * METERS_PER_PIXEL), 0.0f);

	b2FixtureDef sensorFixtureDef;
	sensorFixtureDef.shape = &sensorShape;
	sensorFixtureDef.isSensor = true;

	hitbox->GetBody()->CreateFixture(&sensorFixtureDef);
}

PlayerCannon::~PlayerCannon() {
	UnloadTexture(cannonBaseTexture);
}

void PlayerCannon::Die() {
	if (state != PlayerCannonState::Dead) {
		printf("Player has died.\n");
		state = PlayerCannonState::Dead;
	}
}

bool PlayerCannon::ConsumeShoot() {
	if (shootRequested) {
		shootRequested = false;
		return true;
	}
	return false;
}

void PlayerCannon::SetAction(PlayerCannonAction action, bool active) {
	switch (action) {
	case PlayerCannonAction::RotateLeft:
		actionState.rotateLeft = active;
		break;
	case PlayerCannonAction::RotateRight:
		actionState.rotateRight = active;
		break;
	case PlayerCannonAction::MoveDown:
		actionState.moveDown = active;
		break;
	case PlayerCannonAction::MoveUp:
		actionState.moveUp = active;
		break;
	case PlayerCannonAction::Shoot:
		actionState.shoot = active;
		break;
	case PlayerCannonAction::Push:
		actionState.push = active;
		break;
	case PlayerCannonAction::Pull:
		actionState.pull = active;
		break;
	default:
		break;
	}
}

void PlayerCannon::TeleportTo(float x, float y) {
	hitbox->GetBody()->SetTransform(b2Vec2(x, y), 0.0f);
	hitbox->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
}

void PlayerCannon::Update(float deltaTime) {
	if (context.state != GameState::Playing) {
		b2Body* body = hitbox->GetBody();
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		return;
	}

	if (actionState.shoot) {
		shootRequested = true;
		actionState.shoot = false;
	}

	HandleMovement(deltaTime);
	HandleCannonRotation(deltaTime);

	if (state != PlayerCannonState::Pulling && state != PlayerCannonState::Dead) {
		// TODO: Implement movement logic based on actionState.moveDown and actionState.moveUp
	}

	hitbox->Update(deltaTime);
}

void PlayerCannon::Render(Renderer& renderer) {

	Rectangle srcBase = { 0.0f, 0.0f, (float)cannonBaseTexture.width, (float)cannonBaseTexture.height };
	Rectangle dstBase = { hitbox->position.x - cannonBaseTexture.width / 2.0f, hitbox->position.y - cannonBaseTexture.height / 2.0f, (float)cannonBaseTexture.width, (float)cannonBaseTexture.height };
	renderer.DrawSprite(cannonBaseTexture, srcBase, dstBase, 0.0f, WHITE);

	Rectangle srcTop = { 0.0f, 0.0f, (float)cannonTopTexture.width, (float)cannonTopTexture.height };
	Rectangle dstTop = { hitbox->position.x - 10.0f, hitbox->position.y - cannonTopTexture.height / 2.0f, (float)cannonTopTexture.width, (float)cannonTopTexture.height };
	Vector2 topOrigin = { 10.0f, cannonTopTexture.height / 2.0f };
	renderer.DrawSprite(cannonTopTexture, srcTop, dstTop, topOrigin, hitbox->angle, WHITE);

	if (context.debugMode) {
		hitbox->Render(renderer);
		DrawDebugSensors(renderer);
	}
}

void PlayerCannon::HandleMovement(float deltaTime) {
	if (actionState.moveUp) {
		velocityY -= moveAcceleration * deltaTime;
	}
	else if (actionState.moveDown) {
		velocityY += moveAcceleration * deltaTime;
	}
	else {
		velocityY *= moveDamping;
		if (fabsf(velocityY) < 0.1f) velocityY = 0.0f;
	}

	if (velocityY > maxMoveSpeed) velocityY = maxMoveSpeed;
	if (velocityY < -maxMoveSpeed) velocityY = -maxMoveSpeed;

	b2Body* body = hitbox->GetBody();
	b2Vec2 pos = body->GetPosition();
	pos.y += velocityY * deltaTime;

	float minY = TOP_OFFSET * METERS_PER_PIXEL;
	float maxY = (GetScreenHeight() - BOTTOM_OFFSET) * METERS_PER_PIXEL;
	
	if (pos.y < minY) { pos.y = minY; velocityY = 0.0f; }
	if (pos.y > maxY) { pos.y = maxY; velocityY = 0.0f; }
	
	body->SetTransform(pos, body->GetAngle());
}

void PlayerCannon::HandleCannonRotation(float deltaTime) {
	if (actionState.rotateLeft) {
		hitbox->GetBody()->SetAngularVelocity(-rotationSpeed);
	}
	else if (actionState.rotateRight) {
		hitbox->GetBody()->SetAngularVelocity(rotationSpeed);
	}
	else {
		hitbox->GetBody()->SetAngularVelocity(0.0f);
	}
}

void PlayerCannon::DrawDebugSensors(Renderer& renderer) {
	b2Vec2 pos = hitbox->GetBody()->GetPosition();
	float x = pos.x * PIXELS_PER_METER - PLAYER_WIDTH / 2.0f;
	float y = pos.y * PIXELS_PER_METER + PLAYER_HEIGHT / 2.0f - SENSOR_HEIGHT / 2.0f;
	renderer.DrawRect(x, y, PLAYER_WIDTH, SENSOR_HEIGHT, Fade(RED, 0.5f));
}