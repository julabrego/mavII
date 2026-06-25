#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact* contact)
	{
		printf("Contact began between fixtures: %p and %p\n", contact->GetFixtureA(), contact->GetFixtureB());
		// Handle contact begin event
		// You can access the fixtures and bodies involved in the contact here
	}
	void ContactListener::EndContact(b2Contact* contact)
	{
		printf("Contact ended between fixtures: %p and %p\n", contact->GetFixtureA(), contact->GetFixtureB());
		// Handle contact end event
		// You can access the fixtures and bodies involved in the contact here
	}