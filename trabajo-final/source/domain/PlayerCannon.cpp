#include "PlayerCannon.h"
#include "../core/PhysicsConstants.h"
#include "../core/GameplayConstants.h"
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

	b2Filter filter;
	filter.groupIndex = PLAYER_GROUP_INDEX;
	filter.maskBits = 0;
	hitbox->GetBody()->GetFixtureList()->SetFilterData(filter);

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
	UnloadTexture(armBaseTexture);
	UnloadTexture(aimTexture);
	UnloadTexture(closedHandTexture);
	UnloadTexture(shootTexture);
	UnloadTexture(pullTexture);
	UnloadTexture(ballTexture);
}

void PlayerCannon::Die() {
	isDead = true;
	state = PlayerCannonState::Aiming;
	showBall = true;
}

void PlayerCannon::SetCannonState(PlayerCannonState newState) {
	state = newState;
	showBall = (state == PlayerCannonState::Aiming);
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
	pullRequested = actionState.pull;

	if (pullRequested) state = PlayerCannonState::Pulling;
	else if (shootRequested) state = PlayerCannonState::Shooting;
	else state = PlayerCannonState::Aiming;

	HandleMovement(deltaTime);
	HandleRotation(deltaTime);

	hitbox->Update(deltaTime);
}

void PlayerCannon::Render(Renderer& renderer) {

	Rectangle srcBase = { 0.0f, 0.0f, (float)armBaseTexture.width, (float)armBaseTexture.height };
	b2Vec2 basePos = baseBody->GetPosition();
	float baseX = basePos.x * PIXELS_PER_METER;
	float baseY = basePos.y * PIXELS_PER_METER;
	Rectangle dstBase = { baseX - armBaseTexture.width , baseY - armBaseTexture.height + 4.0f, (float)armBaseTexture.width, (float)armBaseTexture.height };

	Texture2D currentHand = aimTexture;
	if (state == PlayerCannonState::Aiming && showBall) currentHand = aimTexture;
	else if (state == PlayerCannonState::Aiming && !showBall) currentHand = closedHandTexture;
	else if (state == PlayerCannonState::Shooting) currentHand = shootTexture;
	else if (state == PlayerCannonState::Pulling && !showBall) currentHand = pullTexture;

	Rectangle srcTop = { 0.0f, 0.0f, (float)currentHand.width, (float)currentHand.height };
	Vector2 topOrigin = { 5.0f, currentHand.height / 2.0f - 5.0f };
	Rectangle dstTop = { hitbox->position.x - topOrigin.x, hitbox->position.y - currentHand.height / 2.0f, (float)currentHand.width, (float)currentHand.height };

	renderer.DrawSprite(armBaseTexture, srcBase, dstBase, 0.0f, WHITE);
	renderer.DrawSprite(currentHand, srcTop, dstTop, topOrigin, hitbox->angle, WHITE);
	
	if (context.debugMode) {
		hitbox->Render(renderer);
		DrawCircleV(hitbox->position, 4.0f, RED);
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
	float adjustedRotationSpeed = showBall ? rotationSpeed * 0.5f : rotationSpeed;
	if (actionState.rotateLeft) {
		turretJoint->SetMotorSpeed(-adjustedRotationSpeed);
	}
	else if (actionState.rotateRight) {
		turretJoint->SetMotorSpeed(adjustedRotationSpeed);
	}
	else {
		turretJoint->SetMotorSpeed(0.0f);
	}
}