#pragma once
#include <box2d.h>
#include <vector>
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

	bool IsGrounded() const { return groundContactCount > 0; }
	
	int groundContactDelta = 0;
	int groundContactCount = 0;
	
	bool isRotablePlatformTriggered = false;

	std::vector<b2Body*> fallSensorEvents;

private:
	ContactInfo ExtractContactInfo(b2Contact* contact);
};
