#include "PlayerCannon.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"
#include <algorithm>
#include <cmath>

PlayerCannon::PlayerCannon(b2World& world, GameContext& gameContext, float startX, float startY) : context(gameContext) {
	b2Vec2 pivot{ startX * METERS_PER_PIXEL, startY * METERS_PER_PIXEL };

	b2BodyDef baseDef;
	baseDef.type = b2_kinematicBody;
	baseDef.position = pivot;
	baseBody = world.CreateBody(&baseDef);

	hitbox = CircleEntity::CreateDynamic(world, startX, startY, PLAYER_RADIUS, Fade(YELLOW, 0.5f), 1.0f, 0.3f, 0.0f);

	bodyData = { BodyTag::Player, this };
	hitbox->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(&bodyData);
	hitbox->GetBody()->SetGravityScale(0.0f);

	b2RevoluteJointDef turrentJointDef;
	turrentJointDef.Initialize(baseBody, hitbox->GetBody(), pivot);
	turrentJointDef.enableMotor = true;
	turrentJointDef.motorSpeed = 0.0f;
	turrentJointDef.maxMotorTorque = turretMotorTorque;
	turrentJointDef.enableLimit = true;
	turrentJointDef.lowerAngle = MIN_ROTATION_ANGLE;
	turrentJointDef.upperAngle = MAX_ROTATION_ANGLE;
	turretJoint = static_cast<b2RevoluteJoint*>(world.CreateJoint(&turrentJointDef));
}

PlayerCannon::~PlayerCannon() {
	UnloadTexture(cannonBaseTexture);
	UnloadTexture(cannonTopTexture);
}

void PlayerCannon::Die() {
	if (state != PlayerCannonState::Dead) {
		printf("Player has died.\n");
		state = PlayerCannonState::Dead;
	}
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
	case PlayerCannonAction::Pull:
		actionState.pull = active;
		break;
	default:
		break;
	}
}

void PlayerCannon::TeleportTo(float x, float y) {
	baseBody->SetTransform(b2Vec2(x, y), baseBody->GetAngle());
	hitbox->GetBody()->SetTransform(b2Vec2(x, y), hitbox->GetBody()->GetAngle());
	hitbox->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	hitbox->GetBody()->SetAngularVelocity(0.0f);
}

void PlayerCannon::Update(float deltaTime) {
	if (context.state != GameState::Playing) {
		b2Body* body = hitbox->GetBody();
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		return;
	}

	shootRequested = actionState.shoot;

	HandleMovement(deltaTime);
	HandleRotation(deltaTime);

	if (state != PlayerCannonState::Pulling && state != PlayerCannonState::Dead) {
		// TODO: Implement movement logic based on actionState.moveDown and actionState.moveUp
	}

	hitbox->Update(deltaTime);
}

void PlayerCannon::Render(Renderer& renderer) {

	Rectangle srcBase = { 0.0f, 0.0f, (float)cannonBaseTexture.width, (float)cannonBaseTexture.height };
	b2Vec2 basePos = baseBody->GetPosition();
	float baseX = basePos.x * PIXELS_PER_METER;
	float baseY = basePos.y * PIXELS_PER_METER;
	Rectangle dstBase = { baseX - cannonBaseTexture.width / 2.0f, baseY - cannonBaseTexture.height / 2.0f, (float)cannonBaseTexture.width, (float)cannonBaseTexture.height };
	renderer.DrawSprite(cannonBaseTexture, srcBase, dstBase, 0.0f, WHITE);

	Rectangle srcTop = { 0.0f, 0.0f, (float)cannonTopTexture.width, (float)cannonTopTexture.height };
	Rectangle dstTop = { hitbox->position.x - 10.0f, hitbox->position.y - cannonTopTexture.height / 2.0f, (float)cannonTopTexture.width, (float)cannonTopTexture.height };
	Vector2 topOrigin = { 10.0f, cannonTopTexture.height / 2.0f };
	renderer.DrawSprite(cannonTopTexture, srcTop, dstTop, topOrigin, hitbox->angle, WHITE);

	if (context.debugMode) {
		hitbox->Render(renderer);
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

	b2Vec2 pos = baseBody->GetPosition();
	pos.y += velocityY * deltaTime;

	float minY = TOP_OFFSET * METERS_PER_PIXEL;
	float maxY = (GetScreenHeight() - BOTTOM_OFFSET) * METERS_PER_PIXEL;
	
	if (pos.y < minY) { pos.y = minY; velocityY = 0.0f; }
	if (pos.y > maxY) { pos.y = maxY; velocityY = 0.0f; }
	
	baseBody->SetLinearVelocity(b2Vec2(0.0f, velocityY));
}

void PlayerCannon::HandleRotation(float deltaTime) {
	if (actionState.rotateLeft) {
		turretJoint->SetMotorSpeed(-rotationSpeed);
	}
	else if (actionState.rotateRight) {
		turretJoint->SetMotorSpeed(rotationSpeed);
	}
	else {
		turretJoint->SetMotorSpeed(0.0f);
	}
}