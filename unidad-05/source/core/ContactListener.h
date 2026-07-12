#pragma once
#include <box2d.h>

class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

	void ClearFrameEvents();

	bool isRotablePlatformTriggered = false;
	bool playerReachedFinishSensor = false;

	int groundContactDelta = 0;
	bool playerVsEnemyContact = false;
	bool playerStompContact = false;
};

