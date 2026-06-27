#include "ContactListener.h"
#include "BodyData.h"
#include <box2d.h>

void ContactListener::BeginContact(b2Contact* contact)
	{
		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();
		BodyData* dataA = reinterpret_cast<BodyData*>(bodyA->GetUserData().pointer);
		BodyData* dataB = reinterpret_cast<BodyData*>(bodyB->GetUserData().pointer);

		if (!dataA || !dataB) {
			printf("Contact began between fixtures with missing BodyData.\n");
			return;
		}
		
		printf("Contact began between fixtures: %d and %d\n", static_cast<int>(dataA->tag), static_cast<int>(dataB->tag));

		bool playerVsSpring = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Spring) ||
			(dataA->tag == BodyTag::Spring && dataB->tag == BodyTag::Player);
		
		if (playerVsSpring) {
			playerVsSpringContact = true;
		}

		bool playerVsGround = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Ground) || (dataA->tag == BodyTag::Ground && dataB->tag == BodyTag::Player);

		if (playerVsGround) {
			playerVsGroundContact = true;
		}

		bool playerVsTarget = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Target) || (dataA->tag == BodyTag::Target && dataB->tag == BodyTag::Player);

		if (playerVsTarget) {
			playerVsTargetContact = true;
		}
	}
	void ContactListener::EndContact(b2Contact* contact)
	{
		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();
		BodyData* dataA = reinterpret_cast<BodyData*>(bodyA->GetUserData().pointer);
		BodyData* dataB = reinterpret_cast<BodyData*>(bodyB->GetUserData().pointer);

		if(!dataA || !dataB) {
			printf("Contact ended between fixtures with missing BodyData.\n");
			return;
		}

		printf("Contact ended between fixtures: %d and %d\n", static_cast<int>(dataA->tag), static_cast<int>(dataB->tag));

		bool playerVsSpring = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Spring) ||
			(dataA->tag == BodyTag::Spring && dataB->tag == BodyTag::Player);

		if (playerVsSpring) {
			playerVsSpringContact = false;
		}

		bool playerVsGround = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Ground) || (dataA->tag == BodyTag::Ground && dataB->tag == BodyTag::Player);

		if (playerVsGround) {
			playerVsGroundContact = false;
		}

		bool playerVsTarget = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Target) || (dataA->tag == BodyTag::Target && dataB->tag == BodyTag::Player);

		if (playerVsTarget) {
			playerVsTargetContact = false;
		}

	}