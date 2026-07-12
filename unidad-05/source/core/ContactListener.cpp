#include "ContactListener.h"
#include "BodyData.h"
#include <box2d.h>

void ContactListener::ClearFrameEvents()
{
	groundContactDelta = 0;
	playerVsEnemyContact = false;
	playerStompContact = false;
}

void ContactListener::BeginContact(b2Contact* contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	BodyData* dataA = reinterpret_cast<BodyData*>(bodyA->GetUserData().pointer);
	BodyData* dataB = reinterpret_cast<BodyData*>(bodyB->GetUserData().pointer);

	if (!dataA || !dataB) {
		return;
	}

	bool sensorA = contact->GetFixtureA()->IsSensor();
	bool sensorB = contact->GetFixtureB()->IsSensor();
	bool groundA = dataA->tag == BodyTag::Ground || dataA->tag == BodyTag::Box;
	bool groundB = dataB->tag == BodyTag::Ground || dataB->tag == BodyTag::Box;

	if ((sensorA && groundB) || (sensorB && groundA)) {
		b2Body* sensorBody = sensorA ? bodyA : bodyB;
		BodyData* sensorData = reinterpret_cast<BodyData*>(sensorBody->GetUserData().pointer);
		if (sensorData && sensorData->tag == BodyTag::Player) {
			groundContactDelta++;
		}
	}

	bool rotablePlatformTriggererIsInArea = (dataA->tag == BodyTag::RotablePlatformTriggerArea && dataB->tag == BodyTag::RotablePlatformTriggerer)
		|| (dataA->tag == BodyTag::RotablePlatformTriggerer && dataB->tag == BodyTag::RotablePlatformTriggerArea);

	if (rotablePlatformTriggererIsInArea) {
		isRotablePlatformTriggered = true;
	}

	bool playerVsEnemy = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Enemy)
		|| (dataA->tag == BodyTag::Enemy && dataB->tag == BodyTag::Player);

	if (playerVsEnemy && !sensorA && !sensorB) {
		playerVsEnemyContact = true;
	}

	bool playerEnteredFinishSensor = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::FinishSensor)
		|| (dataA->tag == BodyTag::FinishSensor && dataB->tag == BodyTag::Player);

	if (playerEnteredFinishSensor) {
		playerReachedFinishSensor = true;
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	BodyData* dataA = reinterpret_cast<BodyData*>(bodyA->GetUserData().pointer);
	BodyData* dataB = reinterpret_cast<BodyData*>(bodyB->GetUserData().pointer);

	if (!dataA || !dataB) {
		return;
	}

	bool sensorA = contact->GetFixtureA()->IsSensor();
	bool sensorB = contact->GetFixtureB()->IsSensor();
	bool groundA = dataA->tag == BodyTag::Ground || dataA->tag == BodyTag::Box;
	bool groundB = dataB->tag == BodyTag::Ground || dataB->tag == BodyTag::Box;

	if ((sensorA && groundB) || (sensorB && groundA)) {
		b2Body* sensorBody = sensorA ? bodyA : bodyB;
		BodyData* sensorData = reinterpret_cast<BodyData*>(sensorBody->GetUserData().pointer);
		if (sensorData && sensorData->tag == BodyTag::Player) {
			groundContactDelta--;
		}
	}
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	BodyData* dataA = reinterpret_cast<BodyData*>(bodyA->GetUserData().pointer);
	BodyData* dataB = reinterpret_cast<BodyData*>(bodyB->GetUserData().pointer);

	bool isPlayerBodyA = dataA && dataA->tag == BodyTag::Player;
	bool isEnemyBodyA = dataA && dataA->tag == BodyTag::Enemy;
	bool isPlayerBodyB = dataB && dataB->tag == BodyTag::Player;
	bool isEnemyBodyB = dataB && dataB->tag == BodyTag::Enemy;

	if ((isPlayerBodyA && isEnemyBodyB) || (isEnemyBodyA && isPlayerBodyB)) {
		bool fixtureSensorA = contact->GetFixtureA()->IsSensor();
		bool fixtureSensorB = contact->GetFixtureB()->IsSensor();
		if (!fixtureSensorA && !fixtureSensorB) {
			b2Body* playerBody = isPlayerBodyA ? bodyA : bodyB;
			if (playerBody->GetLinearVelocity().y > 1.0f) {
				playerStompContact = true;
			}
		}
	}

	bool playerA = dataA && dataA->tag == BodyTag::Player;
	bool playerB = dataB && dataB->tag == BodyTag::Player;

	if (playerA || playerB)
	{
		if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
			return;

		contact->SetFriction(0.0f);
	}
}
