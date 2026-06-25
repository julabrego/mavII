#include "Scenario.h"
#include "RectangleEntity.h"
#include "CircleEntity.h"
#include "raylib.h"
#include "../core/Colors.h"
#include "../core/Renderer.h"
#include "../core/PhysicsConstants.h"
#include <string>

Scenario::~Scenario() = default;

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
	ground = RectangleEntity::CreateStatic(world, 0.0f, 560.0f, GetScreenWidth(), 40.0f, 0.0f, COLOR_GROUND);
};


void Scenario::CreatePlayableElements(b2World& world) {
	spring = RectangleEntity::CreateStatic(world, 20.0f, ground.get()->position.y - 20.0f, 80.0f, 20.0f, 0.0f, COLOR_SPRING);
	//spring->GetBody()->GetUserData().pointer = spring.get();
}


void Scenario::Update(float deltaTime)
{
	// TODO: wathever->Update(deltaTime);
}

void Scenario::Render(Renderer& renderer)
{
	ground->Render(renderer);
	spring->Render(renderer);

	if (drawJoints) {
		renderer.DrawCenteredText(("Mouse at (" + std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY()) + ")").c_str(), 20, 5, BLACK);
	}
}