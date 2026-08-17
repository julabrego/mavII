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
	CreateBuildingBlocks(world);
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

void Scenario::CreateBuildingBlocks(b2World& world) {
	int numRows = 7;
	int numCols = 2;
	float blockWidth = 45.0f;
	float blockHeight = 45.0f;
	float xInitialBlock = GetScreenWidth() / 2.0f - numCols * blockWidth / 2;
	float yInitialBlock = 540.0f - blockHeight;

	float density = 5.0f;
	float friction = 0.5f;
	float restitution = 0.3f;

	for (int row = 0; row < numRows; ++row) {
		for (int col = 0; col < numCols; ++col) {
			float x = xInitialBlock + col * blockWidth;
			float y = yInitialBlock - row * blockHeight;

			auto block = RectangleEntity::CreateDynamic(world, x, y, blockWidth, blockHeight, 0.0f, COLOR_WALL, density, friction, restitution, 1.0f);
			RegisterBodyData(block->GetBody(), BodyTag::Box, block.get());
			buildingBlocks.push_back(std::move(block));
		}
	}
}

void Scenario::Update(float deltaTime)
{
	for (auto& block : buildingBlocks) {
		block->Update(deltaTime);
	}
}

void Scenario::Render(Renderer& renderer)
{
	ground->Render(renderer);

	for (auto& block : buildingBlocks) {
		block->Render(renderer);
	}
}
