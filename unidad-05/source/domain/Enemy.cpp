#include "Enemy.h"
#include "../core/BodyData.h"
#include "../core/PhysicsConstants.h"

Enemy::Enemy(b2World& world, GameContext& gameContext, float startX, float startY) : context(gameContext)
, world(world) {
	hitbox = RectangleEntity::CreateDynamic(world, startX, startY, ENEMY_WIDTH, ENEMY_HEIGHT, 0.0f, RED, density, friction, bounciness);
	hitbox->GetBody()->SetFixedRotation(true);
	BodyData* enemyData = new BodyData({ BodyTag::Enemy, this });
	hitbox->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(enemyData);

	b2Body* body = hitbox->GetBody();
	float halfW = (ENEMY_WIDTH / 2.0f) * METERS_PER_PIXEL;
	float halfH = (ENEMY_HEIGHT / 2.0f) * METERS_PER_PIXEL;

	// Edge sensors: small box ahead of front-bottom corner
	float edgeAhead = halfW + 5.0f * METERS_PER_PIXEL;
	b2PolygonShape edgeShape;
	edgeShape.SetAsBox(10.0f * METERS_PER_PIXEL, 2.0f * METERS_PER_PIXEL, b2Vec2(edgeAhead, halfH), 0.0f);
	b2FixtureDef edgeFix;
	edgeFix.shape = &edgeShape;
	edgeFix.isSensor = true;
	rightEdgeSensor = body->CreateFixture(&edgeFix);

	edgeShape.SetAsBox(10.0f * METERS_PER_PIXEL, 2.0f * METERS_PER_PIXEL, b2Vec2(-edgeAhead, halfH), 0.0f);
	leftEdgeSensor = body->CreateFixture(&edgeFix);
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
		if (life <= 0) {
			Die();
		}
		else {
			state = EnemyState::Moving;
		}
	}

	if (context.state != GameState::Playing) {
		b2Body* body = hitbox->GetBody();
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		body->SetGravityScale(0.0f);
		return;
	}

	if (state == EnemyState::Moving) {
		b2Body* body = hitbox->GetBody();
		float prevVelX = body->GetLinearVelocity().x;
		body->SetLinearVelocity(b2Vec2(direction * moveSpeed, body->GetLinearVelocity().y));

		float velY = body->GetLinearVelocity().y;
		bool isGrounded = velY > -0.1f && velY < 0.1f;

		if (isGrounded) {
			bool blockedByWall = (prevVelX * direction <= 0.0f && direction != 0.0f) ||
				(prevVelX != 0.0f && fabsf(prevVelX) < moveSpeed * 0.5f);

			// TODO: extract pit detection via edge sensor
			b2Fixture* activeEdge = (direction > 0) ? rightEdgeSensor : leftEdgeSensor;
			bool groundAhead = false;

			for (b2ContactEdge* edge = body->GetContactList(); edge; edge = edge->next) {
				b2Contact* contact = edge->contact;
				if (!contact->IsEnabled()) continue;

				b2Fixture* fixtureA = contact->GetFixtureA();
				b2Fixture* fixtureB = contact->GetFixtureB();

				if (fixtureA == activeEdge || fixtureB == activeEdge) {
					b2Fixture* other = (fixtureA == activeEdge) ? fixtureB : fixtureA;
					BodyData* data = reinterpret_cast<BodyData*>(other->GetBody()->GetUserData().pointer);
					if (data && (data->tag == BodyTag::Ground || data->tag == BodyTag::Box)) {
						groundAhead = true;
					}
				}
			}

			wallCooldown -= deltaTime;
			if (wallCooldown <= 0.0f && blockedByWall) {
				direction *= -1.0f;
				wallCooldown = 0.3f;
			}
			else if (!groundAhead) {
				direction *= -1.0f;
			}
		}
	}

	hitbox->Update(deltaTime);
}

void Enemy::Render(Renderer& renderer) {
	hitbox->Render(renderer);

	if (context.debugMode) {
		DrawDebugSensors();
	}
}

void Enemy::DrawDebugSensors() {
	b2Vec2 bodyPos = hitbox->GetBody()->GetPosition();
	float centerX = bodyPos.x * PIXELS_PER_METER;
	float centerY = bodyPos.y * PIXELS_PER_METER;
	float halfWidth = ENEMY_WIDTH / 2.0f;
	float halfHeight = ENEMY_HEIGHT / 2.0f;
	float sensorOffsetX = halfWidth + 5.0f;
	float sensorWidth = 20.0f;
	float sensorHeight = 4.0f;

	Color activeColor = (direction > 0) ? GREEN : GRAY;
	DrawRectangleLines(centerX + sensorOffsetX - sensorWidth / 2, centerY + halfHeight - sensorHeight / 2, sensorWidth, sensorHeight, activeColor);
	Color inactiveColor = (direction > 0) ? GRAY : GREEN;
	DrawRectangleLines(centerX - sensorOffsetX - sensorWidth / 2, centerY + halfHeight - sensorHeight / 2, sensorWidth, sensorHeight, inactiveColor);
}

