#include "Scenario.h"
#include "RectangleEntity.h"
#include "CircleEntity.h"
#include "Stick.h"
#include "raylib.h"
#include "../core/Colors.h"
#include "../core/Renderer.h"
#include "../core/PhysicsConstants.h"
#include <string>

Scenario::Scenario(b2World& world, float screenWidth, float screenHeight)
{
	CreateBoundaryWalls(world, screenWidth, screenHeight);
	CreateStaticWalls(world);
	CreateBridge(world, 340.0f, 158.0f, 10, 33.0f);
	CreateBridge(world, 610.0f, 158.0f, 10, 33.0f);

	CreatePlayableElements(world);
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
	CreateWall(world, screenWidth + 20.0f, 0.0f, 20.0f, screenHeight);
	CreateWall(world, -20.0f, 0.0f, 20.0f, screenHeight);
};

void Scenario::CreateStaticWalls(b2World& world) {
	dividerWall = RectangleEntity::CreateStatic(world, 672.0f, 158.0f, 45.0f, 471.0f, 0.0f, COLOR_WALL);
	wallLeft = RectangleEntity::CreateStatic(world, 0.0f, 0.0f, 285.0f, GetScreenWidth(), 0.0f, COLOR_WALL);
	wallRight = RectangleEntity::CreateStatic(world, 775.0f, 0.0f, 285.0f, GetScreenHeight(), 0.0f, COLOR_WALL);
	diagonalTopLeft = RectangleEntity::CreateStatic(world, 235.0f, -40.0f, 200.0f, 120.0f, -30.0f, COLOR_WALL);
	diagonalTopRight = RectangleEntity::CreateStatic(world, 625.0f, -40.0f, 200.0f, 120.0f, 30.0f, COLOR_WALL);
	roof = RectangleEntity::CreateStatic(world, 0.0f, 0.0f, GetScreenWidth(), 30.0f, 0.0f, COLOR_WALL);
};

void Scenario::CreateBridge(b2World& world, float startX, float startY, int nodeCount, float nodeSpacing) {
	for (int i = 0; i < nodeCount; ++i) {
		if (i == 0 || i == nodeCount - 1) {
			bridgeNodes.push_back(std::make_unique<CircleEntity>(world, startX, startY + i * nodeSpacing, 5.0f, COLOR_WALL, 0.0f, 0.5f, 0.0f, 2.0f, BLACK, b2_staticBody));
		}
		else {
			bridgeNodes.push_back(std::make_unique<CircleEntity>(world, startX, startY + i * nodeSpacing, 5.0f, COLOR_WALL, 1.0f, 0.5f, 0.0f, 2.0f));
		}
	}
	b2DistanceJointDef bridgeJointDef;
	bridgeJointDef.length = nodeSpacing;
	bridgeJointDef.stiffness = 100.0f;
	for (size_t i = 1; i < bridgeNodes.size(); ++i) {
		bridgeJointDef.Initialize(bridgeNodes[i - 1]->GetBody(), bridgeNodes[i]->GetBody(),
			bridgeNodes[i - 1]->GetBody()->GetWorldCenter(),
			bridgeNodes[i]->GetBody()->GetWorldCenter());
		world.CreateJoint(&bridgeJointDef);
	}
}

void Scenario::CreatePlayableElements(b2World& world) {
	stickLeft = std::make_unique<StickEntity>(world, 360.0f, 525.0f, COLOR_STICK, true);
	stickRight = std::make_unique<StickEntity>(world, 600.0f, 525.0f, COLOR_STICK, false);

	plunger = RectangleEntity::CreateDynamic(world, 722.0f, 525.0f, 48.0f, 10.0f, 0.0f, COLOR_SPRING, 1.0f, 1.0f, 0.0f);
	plunger->GetBody()->SetGravityScale(0.0f);
	plungerIdleY = plunger->GetCenter().y + 20.0f;

	plungerBase = RectangleEntity::CreateStatic(world, 722.0f, GetScreenHeight() - 10.0f, 48.0f, 10.0f, 0.0f, COLOR_STICK);

	b2PrismaticJointDef plungerPrismaticJoint;
	plungerPrismaticJoint.Initialize(plunger->GetBody(), plungerBase->GetBody(),
		b2Vec2({ plunger->GetCenter().x * METERS_PER_PIXEL, plunger->GetCenter().y * METERS_PER_PIXEL }),
		b2Vec2(0.0f, 1.0f));
	plungerPrismaticJoint.enableLimit = true;
	plungerPrismaticJoint.lowerTranslation = -60.0f * METERS_PER_PIXEL;
	plungerPrismaticJoint.upperTranslation = 0.0f;
	plungerPrismaticJoint.enableMotor = true;
	plungerPrismaticJoint.motorSpeed = 0.0f;
	plungerPrismaticJoint.maxMotorForce = 500.0f;
	plungerJoint = (b2PrismaticJoint*)world.CreateJoint(&plungerPrismaticJoint);
}

void Scenario::TriggerSticksAction() {
	stickLeft->TriggerAction();
	stickRight->TriggerAction();
}

void Scenario::ResetSticks() {
	stickLeft->Reset();
	stickRight->Reset();
}

void Scenario::PullPlunger() {
	if (!isPlungerPulled) {
		isPlungerPulled = true;
	}
}

void Scenario::ReleasePlunger() {
	if (isPlungerPulled) {
		isPlungerPulled = false;
	}
}

void Scenario::Update(float deltaTime)
{
	stickLeft->Update(deltaTime);
	stickRight->Update(deltaTime);

	if (isPlungerPulled) {
		plungerJoint->SetMotorSpeed(-80.0f * METERS_PER_PIXEL);
	}
	else {
		plungerJoint->SetMotorSpeed(1000.0f * METERS_PER_PIXEL);
	}

	plunger->Update(deltaTime);

	for (int i = 0; i < bridgeNodes.size(); ++i) {
		bridgeNodes[i]->Update(deltaTime);
	}
}

void Scenario::Render(Renderer& renderer)
{
	dividerWall->Render(renderer);
	wallLeft->Render(renderer);
	wallRight->Render(renderer);
	diagonalTopLeft->Render(renderer);
	diagonalTopRight->Render(renderer);
	roof->Render(renderer);

	stickLeft->Render(renderer);
	stickRight->Render(renderer);

	plunger->Render(renderer);
	plungerBase->Render(renderer);

	for (int i = 0; i < bridgeNodes.size(); ++i) {
		bridgeNodes[i]->Render(renderer);
	}

	if (drawJoints) {
		renderer.DrawCenteredText(("Mouse at (" + std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY()) + ")").c_str(), 20, 5, BLACK);
	}
}