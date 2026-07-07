#include "Scenario.h"

#include "RectangleEntity.h"
#include "CircleEntity.h"
#include "raylib.h"
#include "../core/Colors.h"
#include "../core/Renderer.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"
#include <string>

Scenario::~Scenario() {
	UnloadTexture(finalFlagTexture);
}

Scenario::Scenario(b2World& world, float screenWidth, float screenHeight)
{
	CreateBoundaryWalls(world, screenWidth, screenHeight);
	CreateStaticWalls(world);

}

void Scenario::CreateWall(b2World& world, float x, float y, float halfW, float halfH)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x * METERS_PER_PIXEL, y * METERS_PER_PIXEL);

	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(halfW * METERS_PER_PIXEL, halfH * METERS_PER_PIXEL);

	body->CreateFixture(&shape, 0.0f);
}

void Scenario::CreateBoundaryWalls(b2World& world, float screenWidth, float screenHeight) {
	CreateWall(world, 0.0f, -20.0f, screenWidth, 20.0f);
	CreateWall(world, -20.0f, 0.0f, 20.0f, screenHeight);
};

void Scenario::CreateStaticWalls(b2World& world) {
	// Grounds and static walls
	ground = RectangleEntity::CreateStatic(world, 0.0f, 540.0f, 735.0f, 60.0f, 0.0f, COLOR_GROUND);
	ground2 = RectangleEntity::CreateStatic(world, 900.0f, 540.0f, 105.0f, 60.0f, 0.0f, COLOR_GROUND);
	wall1 = RectangleEntity::CreateStatic(world, 450.0f, 375.0f, 30.0f, 165.0f, 0.0f, COLOR_GROUND);
	ground3 = RectangleEntity::CreateStatic(world, 375.0f, 360.0f, 105.0f, 15.0f, 0.0f, COLOR_GROUND);
	
	BodyData* groundData = new BodyData({ BodyTag::Ground });
	ground->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundData);
	ground2->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundData);
	ground3->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundData);
	
	// Rotable platform
	rotablePlatform = RectangleEntity::CreateDynamic(world, 210.0f, 360.0f, 165.0f, 15.0f, 0.0f, COLOR_STICK, 0.5f, 0.1f, 0.2f);
	b2RevoluteJointDef rotablePlatformJointDef;
	b2Vec2 rotablePlatformInitialPos = b2Vec2((rotablePlatform->GetCenter().x + rotablePlatform->width / 2) * METERS_PER_PIXEL, 
		rotablePlatform->GetCenter().y * METERS_PER_PIXEL);
	rotablePlatformJointDef.Initialize(rotablePlatform->GetBody(), ground3->GetBody(),
		rotablePlatformInitialPos);
	rotablePlatformJointDef.enableLimit = true;
	rotablePlatformJointDef.lowerAngle = 0.0f * DEG2RAD;
	rotablePlatformJointDef.upperAngle = 95.0f * DEG2RAD;

	b2RevoluteJoint* rotablePlatformJoint = (b2RevoluteJoint*)world.CreateJoint(&rotablePlatformJointDef);
	
};


void Scenario::Update(float deltaTime)
{
	rotablePlatform->Update(deltaTime);
}

void Scenario::Render(Renderer& renderer)
{
	ground->Render(renderer);
	ground2->Render(renderer);

	wall1->Render(renderer);
	ground3->Render(renderer);
	rotablePlatform->Render(renderer);

	DrawTexture(finalFlagTexture, 922.5f, 470.0f, WHITE);
}