#include "Scenario.h"
#include "RectangleEntity.h"
#include "CircleEntity.h"
#include "BuildingBlock.h"
#include "raylib.h"
#include "../core/Colors.h"
#include "../core/Renderer.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"
#include "../core/GameplayConstants.h"
#include <string>
#include <cmath>
#include <algorithm>

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
	ground = RectangleEntity::CreateStatic(world, 0.0f, groundTopY, GetScreenWidth(), 60.0f, 0.0f, COLOR_GROUND, 1.0f);

	auto groundData = std::make_unique<BodyData>(BodyTag::Ground);
	BodyData* groundDataPtr = groundData.get();
	bodyDataRegistry.push_back(std::move(groundData));
	ground->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundDataPtr);
}

void Scenario::CreateBuildingBlocks(b2World& world) {
	buildingRows = 7;
	buildingColumns = 2;
	blockWidth = 45.0f;
	blockHeight = 45.0f;
	xInitialBlock = GetScreenWidth() / 2.0f - buildingColumns * blockWidth / 2;
	yInitialBlock = groundTopY - blockHeight;

	float density = 5.0f;
	float friction = 0.5f;
	float restitution = 0.3f;

	for (int row = 0; row < buildingRows; ++row) {
		for (int col = 0; col < buildingColumns; ++col) {
			float x = xInitialBlock + col * blockWidth;
			float y = yInitialBlock - row * blockHeight;

			auto block = BuildingBlock::Create(world, row, col, x, y, blockWidth, blockHeight,
				COLOR_WALL, density, friction, restitution, 1.0f);
			RegisterBodyData(block->GetBody(), BodyTag::BuildingBlock, block.get());
			buildingBlocks.push_back(std::move(block));
		}
	}
}

int Scenario::ComputeBlockLevel(const BuildingBlock& block) const {
	b2Body* body = block.GetBody();
	float bottomY = body->GetPosition().y * PIXELS_PER_METER + blockHeight / 2.0f;
	int level = static_cast<int>(std::lround((groundTopY - bottomY) / blockHeight));
	return level < 0 ? 0 : level;
}

int Scenario::GetCurrentHeight() const {
	Rectangle window = GetCountingWindow();

	bool anyCounted = false;
	int maxLevel = 0;

	for (const auto& block : buildingBlocks) {
		if (!block->IsStanding() && !block->IsLanded()) {
			continue;
		}

		b2Body* body = block->GetBody();
		b2Vec2 center = body->GetPosition();
		float centerX = center.x * PIXELS_PER_METER;
		if (centerX < window.x || centerX > window.x + window.width) {
			continue;
		}

		int level = ComputeBlockLevel(*block);

		anyCounted = true;
		maxLevel = std::max(maxLevel, level);
	}

	return anyCounted ? maxLevel + 1 : 0;
}

bool Scenario::HaveAllFallenBlocksLanded() const {
	for (const auto& block : buildingBlocks) {
		if (!block->IsStanding() && !block->IsLanded()) {
			return false;
		}
	}
	return true;
}

Rectangle Scenario::GetCountingWindow() const {
	float margin = blockWidth * BLOCK_FOOTPRINT_MARGIN_RATIO;
	float left = xInitialBlock - margin;
	float right = xInitialBlock + buildingColumns * blockWidth + margin;
	return Rectangle{ left, 0.0f, right - left, groundTopY };
}

void Scenario::Update(float deltaTime, int buildingHeightTarget)
{
	Rectangle window = GetCountingWindow();

	for (auto& block : buildingBlocks) {
		block->Update(deltaTime);

		bool shadowed = false;
		if (block->IsLanded()) {
			float centerX = block->GetBody()->GetPosition().x * PIXELS_PER_METER;
			bool inWindow = centerX >= window.x && centerX <= window.x + window.width;
			bool augmentsCount = inWindow && ComputeBlockLevel(*block) >= buildingHeightTarget;
			shadowed = !augmentsCount;
		}

		block->SetShadowed(shadowed);
	}
}

void Scenario::Render(Renderer& renderer, int buildingHeightTarget)
{
	ground->Render(renderer);

	for (auto& block : buildingBlocks) {
		block->Render(renderer);
	}

	if (buildingHeightTarget > 0 && context.state != GameState::MainMenu) {
		Rectangle window = GetCountingWindow();
		float goalY = groundTopY - buildingHeightTarget * blockHeight;
		DrawDashedHLine(renderer, goalY, window.x, window.x + window.width, COLOR_DANGER);
		renderer.DrawText("GOAL",
			static_cast<int>(window.x + window.width) + 8,
			static_cast<int>(goalY - 12), 20, COLOR_DANGER);
	}
}

void Scenario::DrawDashedHLine(Renderer& renderer, float y, float x1, float x2, Color color) const {
	const float dashLength = 10.0f;
	const float gapLength = 6.0f;

	for (float x = x1; x < x2; x += dashLength + gapLength) {
		float dashEnd = std::min(x + dashLength, x2);
		renderer.DrawLine(x, y, dashEnd, y, 3.0f, color);
	}
}
