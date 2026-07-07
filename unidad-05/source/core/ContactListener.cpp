#include "ContactListener.h"
#include "BodyData.h"
#include "../domain/Player.h"
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

		bool sensorA = contact->GetFixtureA()->IsSensor();
		bool sensorB = contact->GetFixtureB()->IsSensor();
		bool groundA = dataA->tag == BodyTag::Ground;
		bool groundB = dataB->tag == BodyTag::Ground;

		if ((sensorA && groundB) || (sensorB && groundA)) {
			b2Body* playerBody = sensorA ? bodyA : bodyB;
			BodyData* playerData = reinterpret_cast<BodyData*>(playerBody->GetUserData().pointer);
			Player* player = reinterpret_cast<Player*>(playerData->entity);
			player->SetGrounded(true);
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

		bool sensorA = contact->GetFixtureA()->IsSensor();
		bool sensorB = contact->GetFixtureB()->IsSensor();
		bool groundA = dataA->tag == BodyTag::Ground;
		bool groundB = dataB->tag == BodyTag::Ground;

		if ((sensorA && groundB) || (sensorB && groundA)) {
			b2Body* playerBody = sensorA ? bodyA : bodyB;
			BodyData* playerData = reinterpret_cast<BodyData*>(playerBody->GetUserData().pointer);
			Player* player = reinterpret_cast<Player*>(playerData->entity);
			player->SetGrounded(false);
		}

		bool playerVsTarget = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Target) || (dataA->tag == BodyTag::Target && dataB->tag == BodyTag::Player);

		if (playerVsTarget) {
			playerVsTargetContact = false;
		}

	}