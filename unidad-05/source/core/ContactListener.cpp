#include "ContactListener.h"
#include "BodyData.h"
#include <box2d.h>

void ContactListener::ClearFrameEvents()
{
	groundContactDelta = 0;
	playerVsEnemyContact = false;
	playerStompContact = false;
}

ContactInfo ContactListener::ExtractContactInfo(b2Contact* contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	return {
		bodyA,
		bodyB,
		reinterpret_cast<BodyData*>(bodyA->GetUserData().pointer),
		reinterpret_cast<BodyData*>(bodyB->GetUserData().pointer),
		contact->GetFixtureA()->IsSensor(),
		contact->GetFixtureB()->IsSensor()
	};
}

void ContactListener::BeginContact(b2Contact* contact)
{
	ContactInfo info = ExtractContactInfo(contact);
	if (!info.dataA || !info.dataB) return;

	HandlePlayerGroundContact(contact, info, +1);
	HandleRotablePlatformContacts(info);
	HandlePlayerVsEnemyContact(info);
	HandlePlayerVsFinishedSensorContact(info);
}

void ContactListener::EndContact(b2Contact* contact)
{
	ContactInfo info = ExtractContactInfo(contact);
	if (!info.dataA || !info.dataB) return;

	HandlePlayerGroundContact(contact, info, -1);
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	ContactInfo info = ExtractContactInfo(contact);
	if (!info.dataA || !info.dataB) return;

	HandleEnemyStompContact(info);
	HandlePlayerAgainstWallFrictionContact(contact, info);
}

void ContactListener::HandlePlayerGroundContact(b2Contact* contact, const ContactInfo& info, int sign) {
	bool groundA = info.dataA->tag == BodyTag::Ground || info.dataA->tag == BodyTag::Box;
	bool groundB = info.dataB->tag == BodyTag::Ground || info.dataB->tag == BodyTag::Box;

	if ((info.sensorA && groundB) || (info.sensorB && groundA)) {
		b2Body* sensorBody = info.sensorA ? info.bodyA : info.bodyB;
		BodyData* sensorData = reinterpret_cast<BodyData*>(sensorBody->GetUserData().pointer);
		if (sensorData && sensorData->tag == BodyTag::Player) {
			groundContactDelta += sign;
		}
	}
}

void ContactListener::HandleRotablePlatformContacts(const ContactInfo& info) {
	bool trigger = (info.dataA->tag == BodyTag::RotablePlatformTriggerArea && info.dataB->tag == BodyTag::RotablePlatformTriggerer)
		|| (info.dataA->tag == BodyTag::RotablePlatformTriggerer && info.dataB->tag == BodyTag::RotablePlatformTriggerArea);

	if (trigger) {
		isRotablePlatformTriggered = true;
	}
}

void ContactListener::HandlePlayerVsEnemyContact(const ContactInfo& info) {
	bool playerVsEnemy = (info.dataA->tag == BodyTag::Player && info.dataB->tag == BodyTag::Enemy)
		|| (info.dataA->tag == BodyTag::Enemy && info.dataB->tag == BodyTag::Player);

	if (playerVsEnemy && !info.sensorA && !info.sensorB) {
		playerVsEnemyContact = true;
	}
}

void ContactListener::HandleEnemyStompContact(const ContactInfo& info) {
	bool isPlayerA = info.dataA->tag == BodyTag::Player;
	bool isEnemyA = info.dataA->tag == BodyTag::Enemy;
	bool isPlayerB = info.dataB->tag == BodyTag::Player;
	bool isEnemyB = info.dataB->tag == BodyTag::Enemy;

	if ((isPlayerA && isEnemyB) || (isEnemyA && isPlayerB)) {
		if (!info.sensorA && !info.sensorB) {
			b2Body* playerBody = isPlayerA ? info.bodyA : info.bodyB;
			if (playerBody->GetLinearVelocity().y > 1.0f) {
				playerStompContact = true;
			}
		}
	}
}

void ContactListener::HandlePlayerAgainstWallFrictionContact(b2Contact* contact, const ContactInfo& info) {
	bool isPlayerA = info.dataA->tag == BodyTag::Player;
	bool isPlayerB = info.dataB->tag == BodyTag::Player;

	if (isPlayerA || isPlayerB) {
		if (info.sensorA || info.sensorB) return;
		contact->SetFriction(0.0f);
	}
}

void ContactListener::HandlePlayerVsFinishedSensorContact(const ContactInfo& info) {
	bool finish = (info.dataA->tag == BodyTag::Player && info.dataB->tag == BodyTag::FinishSensor)
		|| (info.dataA->tag == BodyTag::FinishSensor && info.dataB->tag == BodyTag::Player);

	if (finish) {
		playerReachedFinishSensor = true;
	}
}
