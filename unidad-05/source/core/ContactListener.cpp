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

		bool playerSensorVsGround = (dataA->tag == BodyTag::PlayerGroundSensor && dataB->tag == BodyTag::Ground) || (dataA->tag == BodyTag::Ground && dataB->tag == BodyTag::PlayerGroundSensor);

		if (playerSensorVsGround) {
			BodyData* playerSensorData = (dataA->tag == BodyTag::PlayerGroundSensor) ? dataA : dataB;
			Player* player = reinterpret_cast<Player*>(playerSensorData->entity);
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

		bool playerGroundSensorVsGround = (dataA->tag == BodyTag::PlayerGroundSensor && dataB->tag == BodyTag::Ground) || (dataA->tag == BodyTag::Ground && dataB->tag == BodyTag::PlayerGroundSensor);

		if (playerGroundSensorVsGround) {
			BodyData* playerSensorData = (dataA->tag == BodyTag::PlayerGroundSensor) ? dataA : dataB;
			Player* player = reinterpret_cast<Player*>(playerSensorData->entity);
			player->SetGrounded(false);
		}

		bool playerVsTarget = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Target) || (dataA->tag == BodyTag::Target && dataB->tag == BodyTag::Player);

		if (playerVsTarget) {
			playerVsTargetContact = false;
		}

	}