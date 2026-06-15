#include "Scenario.h"
#include "RectangleEntity.h"
#include "CircleEntity.h"
#include "Stick.h"
#include "raylib.h"
#include "../core/Colors.h"
#include "../core/Renderer.h"
#include <string>

Scenario::Scenario(b2World& world, float screenWidth, float screenHeight)
{
	CreateBoundaryWalls(world, screenWidth, screenHeight);
	CreateStaticWalls(world);

	CreatePlayableElements(world);
}

void Scenario::CreateWall(b2World& world, float x, float y, float halfW, float halfH)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);

	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(halfW, halfH);

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

void Scenario::CreatePlayableElements(b2World& world) {
	stickLeft = std::make_unique<StickEntity>(world, 360.0f, 552.0f, COLOR_STICK);
	//stickRight = std::make_unique<StickEntity>(world, 500.0f, 300.0f, COLOR_STICK);
}

void Scenario::TriggerSticksAction() {
	stickLeft->TriggerAction();
	//stickRight->TriggerAction();
}

void Scenario::ResetSticks() {
	stickLeft->Reset();
	//stickRight->Reset();
}

void Scenario::Update(float deltaTime)
{
	stickLeft->Update(deltaTime);
	//wall1->Update(deltaTime);
	//wallLeft->Update(deltaTime);
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

	if (drawJoints) {
		renderer.DrawCenteredText(("Mouse at (" + std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY()) + ")").c_str(), 20, 5, BLACK);
	}
}
