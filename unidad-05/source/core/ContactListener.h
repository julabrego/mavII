#pragma once
#include <box2d.h>
#include "BodyData.h"

struct ContactInfo {
    b2Body* bodyA;
    b2Body* bodyB;
    BodyData* dataA;
    BodyData* dataB;
    bool sensorA;
    bool sensorB;
};

class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	void ClearFrameEvents();

	int groundContactDelta = 0;
	
	bool isRotablePlatformTriggered = false;
	bool playerReachedFinishSensor = false;
	bool playerVsEnemyContact = false;
	bool playerStompContact = false;

private:
	ContactInfo ExtractContactInfo(b2Contact* contact);

	void HandlePlayerGroundContact(b2Contact* contact, const ContactInfo& info, int sign);
	void HandleRotablePlatformContacts(const ContactInfo& info);
	void HandlePlayerVsEnemyContact(const ContactInfo& info);
	void HandleEnemyStompContact(const ContactInfo& info);
	void HandlePlayerVsFinishedSensorContact(const ContactInfo& info);
};
