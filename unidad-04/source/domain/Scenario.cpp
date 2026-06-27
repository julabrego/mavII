#include "Scenario.h"
#include "RectangleEntity.h"
#include "CircleEntity.h"
#include "raylib.h"
#include "../core/Colors.h"
#include "../core/Renderer.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"
#include <string>

Scenario::~Scenario() = default;

Scenario::Scenario(b2World& world, float screenWidth, float screenHeight)
{
	CreateBoundaryWalls(world, screenWidth, screenHeight);
	CreateStaticWalls(world);
	CreatePlayableElements(world);
	CreateTargets(world);
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
	ground = RectangleEntity::CreateStatic(world, 0.0f, 560.0f, GetScreenWidth(), 40.0f, 0.0f, COLOR_GROUND);
	
	BodyData* groundData = new BodyData({ BodyTag::Ground });
	ground->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundData);
};

void Scenario::CreateTargets(b2World& world) {
	float positions[][2] = {
		{540.0f, 51.0f},
		{854.0f, 110.0f},
		{696.0f, 169.0f},
		{540.0f, 228.0f},
		{696.0f, 347.0f},
		{854.0f, 288.0f},
		{540.0f, 406.0f},
		{854.0f, 465.0f}
	};

	for (int i = 0; i < 8; ++i) {
		targets.push_back(std::make_unique<CircleEntity>(
			world, positions[i][0], positions[i][1], 15.0f, RED,
			0.0f, 0.0f, 1.5f, 0.0f, BLACK, b2_staticBody
		));
		BodyData* targetData = new BodyData({ BodyTag::Target });
		targets[i]->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(targetData);
	}
}

void Scenario::CreatePlayableElements(b2World& world) {
	spring = RectangleEntity::CreateStatic(world, 20.0f, ground.get()->position.y - 20.0f, 80.0f, 20.0f, 0.0f, COLOR_SPRING);
	
	BodyData* springData = new BodyData({ BodyTag::Spring });
	spring->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(springData);
}


void Scenario::Update(float deltaTime)
{
	// TODO: wathever->Update(deltaTime);
}

void Scenario::Render(Renderer& renderer)
{
	ground->Render(renderer);
	spring->Render(renderer);

	for (auto& target : targets) {
		target->Render(renderer);
	}

	if (drawJoints) {
		renderer.DrawCenteredText(("Mouse at (" + std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY()) + ")").c_str(), 20, 5, BLACK);
	}
}