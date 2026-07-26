#include "ContactListener.h"
#include "BodyData.h"
#include <box2d.h>

void ContactListener::ClearFrameEvents()
{
	groundContactCount += groundContactDelta;
	groundContactDelta = 0;
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

//void ContactListener::HandleRotablePlatformContacts(const ContactInfo& info) {
//	bool trigger = (info.dataA->tag == BodyTag::RotablePlatformTriggerArea && info.dataB->tag == BodyTag::RotablePlatformTriggerer)
//		|| (info.dataA->tag == BodyTag::RotablePlatformTriggerer && info.dataB->tag == BodyTag::RotablePlatformTriggerArea);
//
//	if (trigger) {
//		isRotablePlatformTriggered = true;
//	}
//}