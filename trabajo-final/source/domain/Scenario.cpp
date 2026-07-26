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
	//UnloadTexture(finalFlagTexture);
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
	ground = RectangleEntity::CreateStatic(world, 0.0f, 540.0f, GetScreenWidth(), 60.0f, 0.0f, COLOR_GROUND, 1.0f);

	auto groundData = std::make_unique<BodyData>(BodyTag::Ground);
	BodyData* groundDataPtr = groundData.get();
	bodyDataRegistry.push_back(std::move(groundData));
	ground->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundDataPtr);
}



void Scenario::Update(float deltaTime)
{
	// TODO: update dynamic elements if needed
}

void Scenario::Render(Renderer& renderer)
{
	ground->Render(renderer);
}
