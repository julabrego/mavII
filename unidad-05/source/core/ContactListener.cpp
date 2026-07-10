#include "ContactListener.h"
#include "BodyData.h"
#include "../domain/Player.h"
#include "../domain/Enemy.h"
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

		// TODO: extract: player in ground flag
		bool sensorA = contact->GetFixtureA()->IsSensor();
		bool sensorB = contact->GetFixtureB()->IsSensor();
		bool groundA = dataA->tag == BodyTag::Ground || dataA->tag == BodyTag::Box;
		bool groundB = dataB->tag == BodyTag::Ground || dataB->tag == BodyTag::Box;

		if ((sensorA && groundB) || (sensorB && groundA)) {
			b2Body* sensorBody = sensorA ? bodyA : bodyB;
			BodyData* sensorData = reinterpret_cast<BodyData*>(sensorBody->GetUserData().pointer);
			if (sensorData && sensorData->tag == BodyTag::Player && sensorData->entity) {
				Player* player = reinterpret_cast<Player*>(sensorData->entity);
				player->IncrementGroundContacts();
			}
		}

		bool playerVsTarget = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Target) || (dataA->tag == BodyTag::Target && dataB->tag == BodyTag::Player);

		if (playerVsTarget) {
			playerVsTargetContact = true;
		}

		// TODO: extract: rotable platform trigger area vs rotable platform triggerer
		bool rotablePlatformTriggererIsInArea = (dataA->tag == BodyTag::RotablePlatformTriggerArea && dataB->tag == BodyTag::RotablePlatformTriggerer) || (dataA->tag == BodyTag::RotablePlatformTriggerer && dataB->tag == BodyTag::RotablePlatformTriggerArea);

		if (rotablePlatformTriggererIsInArea) {
			isRotablePlatformTriggered = true;
		}

		bool playerVsEnemy = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Enemy) 
			|| (dataA->tag == BodyTag::Enemy && dataB->tag == BodyTag::Player);

		if (playerVsEnemy && !sensorA && !sensorB) {
			Player* player = (dataA->tag == BodyTag::Player) ? reinterpret_cast<Player*>(dataA->entity) : reinterpret_cast<Player*>(dataB->entity);
			if (player->GetState() != PlayerState::Falling) {
				player->TakeDamage();
			}
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
		bool groundA = dataA->tag == BodyTag::Ground || dataA->tag == BodyTag::Box;
		bool groundB = dataB->tag == BodyTag::Ground || dataB->tag == BodyTag::Box;

		if ((sensorA && groundB) || (sensorB && groundA)) {
			b2Body* sensorBody = sensorA ? bodyA : bodyB;
			BodyData* sensorData = reinterpret_cast<BodyData*>(sensorBody->GetUserData().pointer);
			if (sensorData && sensorData->tag == BodyTag::Player && sensorData->entity) {
				Player* player = reinterpret_cast<Player*>(sensorData->entity);
				player->DecrementGroundContacts();
			}
		}

		bool playerVsTarget = (dataA->tag == BodyTag::Player && dataB->tag == BodyTag::Target) || (dataA->tag == BodyTag::Target && dataB->tag == BodyTag::Player);

		if (playerVsTarget) {
			playerVsTargetContact = false;
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
				BodyData* playerData = isPlayerBodyA ? dataA : dataB;
				BodyData* enemyData = isEnemyBodyA ? dataA : dataB;
				if (playerBody->GetLinearVelocity().y > 1.0f) {
					Player* player = reinterpret_cast<Player*>(playerData->entity);
					player->Bounce();
					Enemy* enemy = reinterpret_cast<Enemy*>(enemyData->entity);
					enemy->TakeDamage();
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