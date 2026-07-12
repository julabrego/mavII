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

void Scenario::RegisterBodyData(b2Body* body, BodyTag tag, void* entity) {
	auto data = std::make_unique<BodyData>(tag, entity);
	body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data.get());
	bodyDataRegistry.push_back(std::move(data));
}

Scenario::Scenario(b2World& world, GameContext& gameContext, float screenWidth, float screenHeight) 
	: context(gameContext)
{
	CreateBoundaryWalls(world, screenWidth, screenHeight);
	CreateGroundsAndWalls(world);
	CreateRotablePlatform(world);
	CreateDynamicElements(world);
	CreateFinishSensor(world);
}

b2Body* Scenario::CreateWall(b2World& world, float x, float y, float halfW, float halfH)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x * METERS_PER_PIXEL, y * METERS_PER_PIXEL);

	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(halfW * METERS_PER_PIXEL, halfH * METERS_PER_PIXEL);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);
	return body;
}

void Scenario::CreateBoundaryWalls(b2World& world, float screenWidth, float screenHeight) {
	RegisterBodyData(CreateWall(world, 0.0f, -20.0f, screenWidth, 20.0f), BodyTag::Wall);
	RegisterBodyData(CreateWall(world, screenWidth + 20.0f, 0.0f, 20.0f, screenHeight), BodyTag::Wall);
	RegisterBodyData(CreateWall(world, -20.0f, 0.0f, 20.0f, screenHeight), BodyTag::Wall);
}

void Scenario::CreateGroundsAndWalls(b2World& world) {
	ground = RectangleEntity::CreateStatic(world, 0.0f, 540.0f, 735.0f, 60.0f, 0.0f, COLOR_GROUND);
	ground2 = RectangleEntity::CreateStatic(world, 900.0f, 540.0f, 105.0f, 60.0f, 0.0f, COLOR_GROUND);
	wall1 = RectangleEntity::CreateStatic(world, 450.0f, 375.0f, 30.0f, 165.0f, 0.0f, COLOR_GROUND);
	ground3 = RectangleEntity::CreateStatic(world, 375.0f, 360.0f, 105.0f, 15.0f, 0.0f, COLOR_GROUND);

	auto groundData = std::make_unique<BodyData>(BodyTag::Ground);
	BodyData* groundDataPtr = groundData.get();
	bodyDataRegistry.push_back(std::move(groundData));
	ground->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundDataPtr);
	ground2->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundDataPtr);
	ground3->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundDataPtr);
	RegisterBodyData(wall1->GetBody(), BodyTag::Wall);
}

void Scenario::CreateRotablePlatform(b2World& world) {
	rotablePlatform = RectangleEntity::CreateDynamic(world, 210.0f, 360.0f, 165.0f, 15.0f, 0.0f, COLOR_STICK, 0.5f, 0.0f, 0.2f);
	b2RevoluteJointDef rotablePlatformJointDef;
	b2Vec2 rotablePlatformInitialPos = b2Vec2((rotablePlatform->GetCenter().x + rotablePlatform->width / 2) * METERS_PER_PIXEL,
		rotablePlatform->GetCenter().y * METERS_PER_PIXEL);
	rotablePlatformJointDef.Initialize(rotablePlatform->GetBody(), ground3->GetBody(),
		rotablePlatformInitialPos);
	rotablePlatformJointDef.enableLimit = true;
	rotablePlatformJointDef.lowerAngle = 0.0f;
	rotablePlatformJointDef.upperAngle = 0.0f;

	world.CreateJoint(&rotablePlatformJointDef);

	movableWall = RectangleEntity::CreateDynamic(world, 210.0f, 375.0f, 15.0f, 160.0f, 0.0f, COLOR_STICK, 10.0f, 0.0f, 0.0f);
	movableWall->GetBody()->SetFixedRotation(true);
	RegisterBodyData(movableWall->GetBody(), BodyTag::RotablePlatformTriggerer);
	movableWall->GetBody()->SetLinearDamping(5.0f);
}

void Scenario::CreateDynamicElements(b2World& world) {
	box1 = RectangleEntity::CreateDynamic(world, 285.0f, 315.0f, 60.0f, 60.0f, 0.0f, ORANGE, 1.0f, 0.0f, 0.0f);
	box1->GetBody()->SetLinearDamping(1.5f);
	box1->GetBody()->SetAngularDamping(2.0f);
	RegisterBodyData(box1->GetBody(), BodyTag::Box);

	obstacleSensor1 = RectangleEntity::CreateSensor(world, 390.0f, 405.0f, 60.0f, 135.0f, 0.0f, Fade(Fade(GREEN, 0.5f), 0.5f));
	RegisterBodyData(obstacleSensor1->GetBody(), BodyTag::RotablePlatformTriggerArea);
}

void Scenario::CreateFinishSensor(b2World& world) {
	finishSensor = RectangleEntity::CreateSensor(world, 930.0f, 510.0f, 60.0f, 30.0f, 0.0f, Fade(Fade(GREEN, 0.5f), 0.5f));
	RegisterBodyData(finishSensor->GetBody(), BodyTag::FinishSensor);
}

void Scenario::EnablePlatformRotation()
{
	b2RevoluteJoint* rotablePlatformJoint = (b2RevoluteJoint*)rotablePlatform->GetBody()->GetJointList()->joint;
	rotablePlatformJoint->EnableLimit(false);
}

void Scenario::Update(float deltaTime)
{
	rotablePlatform->Update(deltaTime);
	movableWall->Update(deltaTime);
	box1->Update(deltaTime);
}

void Scenario::Render(Renderer& renderer)
{
	ground->Render(renderer);
	ground2->Render(renderer);
	wall1->Render(renderer);
	ground3->Render(renderer);

	rotablePlatform->Render(renderer);
	movableWall->Render(renderer);
	box1->Render(renderer);

	renderer.DrawSprite(finalFlagTexture, { 0, 0, (float)finalFlagTexture.width, (float)finalFlagTexture.height }, { 922.5f, 470.0f, (float)finalFlagTexture.width, (float)finalFlagTexture.height }, WHITE);

	if(context.debugMode) {
		obstacleSensor1->Render(renderer);
		finishSensor->Render(renderer);
	}
}
