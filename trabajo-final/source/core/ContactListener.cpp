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
}

void ContactListener::EndContact(b2Contact* contact)
{
	ContactInfo info = ExtractContactInfo(contact);
	if (!info.dataA || !info.dataB) return;
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	ContactInfo info = ExtractContactInfo(contact);
	if (!info.dataA || !info.dataB) return;
}
