#pragma once
#include <box2d.h>
#include <stdio.h>

class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	bool playerGroundSensorVsGroundContact = false;
	bool playerVsTargetContact = false;
};

