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
	if (blockTexture.id > 0) {
		UnloadTexture(blockTexture);
	}
}

void Scenario::RegisterBodyData(b2Body* body, BodyTag tag, void* entity) {
	auto data = std::make_unique<BodyData>(tag, entity);
	body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data.get());
	bodyDataRegistry.push_back(std::move(data));
}

Scenario::Scenario(b2World& world, GameContext& gameContext, const LevelConfig& levelConfig) 
	: context(gameContext)
	, config(levelConfig)
	, blockSize(levelConfig.blockSize)
	, groundTopY(levelConfig.groundTopY)
{
	buildingRows = static_cast<int>(config.pattern.size());
	for (const auto& row : config.pattern) {
		buildingColumns = std::max(buildingColumns, static_cast<int>(row.size()));
	}

	float buildingWidth = buildingColumns * blockSize;
	float centerX = config.buildingCenterX > 0.0f ? config.buildingCenterX : GetScreenWidth() / 2.0f;
	xInitialBlock = centerX - buildingWidth / 2.0f;

	CreateBoundaryWalls(world);

	if (!config.texturePath.empty()) {
		blockTexture = LoadTexture(config.texturePath.c_str());
	}

	CreateBuildingBlocks(world);
	CreateGround(world);
	CreateObstacles(world);
	CreatePrismaticWalls(world);
	CreateFallSensor(world, config.heightTarget);
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

void Scenario::CreateBoundaryWalls(b2World& world) {
	float screenWidth = static_cast<float>(GetScreenWidth());
	float screenHeight = static_cast<float>(GetScreenHeight());

	RegisterBodyData(CreateWall(world, 0.0f, -20.0f, screenWidth, 20.0f), BodyTag::Wall);
	RegisterBodyData(CreateWall(world, screenWidth + 20.0f, 0.0f, 20.0f, screenHeight), BodyTag::Wall);
	RegisterBodyData(CreateWall(world, -20.0f, 0.0f, 20.0f, screenHeight), BodyTag::Wall);
}

void Scenario::CreateGround(b2World& world) {
	ground = RectangleEntity::CreateStatic(world, 0.0f, groundTopY, static_cast<float>(GetScreenWidth()), 60.0f, 0.0f, COLOR_GROUND, 1.0f);

	auto groundData = std::make_unique<BodyData>(BodyTag::Ground);
	BodyData* groundDataPtr = groundData.get();
	bodyDataRegistry.push_back(std::move(groundData));
	ground->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(groundDataPtr);
}

void Scenario::CreateBuildingBlocks(b2World& world) {
	float density = 5.0f;
	float friction = 0.5f;
	float restitution = 0.3f;

	for (int patternRow = 0; patternRow < buildingRows; ++patternRow) {
		const std::string& row = config.pattern[patternRow];
		int rowFromBottom = buildingRows - 1 - patternRow;

		for (int col = 0; col < static_cast<int>(row.size()); ++col) {
			if (row[col] != 'X') {
				continue;
			}

			float x = xInitialBlock + col * blockSize;
			float y = groundTopY - (rowFromBottom + 1) * blockSize;

			auto block = BuildingBlock::Create(world, rowFromBottom, col, x, y, blockSize, blockSize, COLOR_WALL, density, friction, restitution, 1.0f);

			if (blockTexture.id > 0 && config.textureCols > 0 && config.textureRows > 0) {
				block->SetTexture(&blockTexture, config.textureCols, config.textureRows, col, patternRow);
			}

			RegisterBodyData(block->GetBody(), BodyTag::BuildingBlock, block.get());
			buildingBlocks.push_back(std::move(block));
		}
	}
}

void Scenario::CreateObstacles(b2World& world) {
	for (const auto& obstacleDef : config.obstacles) {
		auto obstacle = RectangleEntity::CreateStatic(world, obstacleDef.x, obstacleDef.y,
		obstacleDef.w, obstacleDef.h, 0.0f, COLOR_WALL, 0.5f, 1.0f);
		RegisterBodyData(obstacle->GetBody(), BodyTag::Obstacle);
		obstacles.push_back(std::move(obstacle));
	}
}

void Scenario::ScanPrismaticPattern(std::vector<std::vector<int>>& wallByCol, std::vector<std::vector<int>>& wallTargetByCol) const {
	for (int patternRow = 0; patternRow < buildingRows; ++patternRow) {
		const std::string& row = config.pattern[patternRow];
		for (int col = 0; col < static_cast<int>(row.size()); ++col) {
			if (row[col] == '|') {
				wallByCol[col].push_back(patternRow);
			}
			else if (row[col] == 'P') {
				wallTargetByCol[col].push_back(patternRow);
			}
		}
	}
}

void Scenario::SpawnPrismaticWall(b2World& world, int col, const std::vector<int>& wallRows, const std::vector<int>& wallTargetRows) {
	int wallTopRow = wallRows.front();
	int wallBottomRow = wallRows.back();
	int wallCount = wallBottomRow - wallTopRow + 1;

	int wallTargetTopRow = wallTargetRows.front();
	int wallTargetBottomRow = wallTargetRows.back();
	int wallTargetCount = wallTargetBottomRow - wallTargetTopRow + 1;

	float wallWidth = blockSize;
	float wallHeight = wallCount * blockSize;
	float wallX = xInitialBlock + col * blockSize;

	int topRowFromBottom = buildingRows - 1 - wallTopRow;
	float wallStartY = groundTopY - (topRowFromBottom + 1) * blockSize + wallHeight / 2.0f;

	int wallTargetTopRowFromBottom = buildingRows - 1 - wallTargetTopRow;
	float wallTargetHeight = wallTargetCount * blockSize;
	float wallTargetY = groundTopY - (wallTargetTopRowFromBottom + 1) * blockSize + wallTargetHeight / 2.0f;

	float travelPx = wallTargetY - wallStartY;
	float halfW = wallWidth / 2.0f;
	float halfH = wallHeight / 2.0f;
	float centerX = wallX + halfW;
	float centerY = wallStartY;

	b2BodyDef wallBodyDef;
	wallBodyDef.type = b2_dynamicBody;
	wallBodyDef.position.Set(centerX * METERS_PER_PIXEL, centerY * METERS_PER_PIXEL);
	wallBodyDef.gravityScale = 0.0f;
	b2Body* wallBody = world.CreateBody(&wallBodyDef);

	b2PolygonShape wallShape;
	wallShape.SetAsBox(halfW * METERS_PER_PIXEL, halfH * METERS_PER_PIXEL);

	b2FixtureDef wallFix;
	wallFix.shape = &wallShape;
	wallFix.density = 1.0f;
	wallFix.friction = 0.5f;
	wallBody->CreateFixture(&wallFix);

	b2BodyDef anchorDef;
	anchorDef.type = b2_staticBody;
	anchorDef.position = wallBody->GetPosition();
	b2Body* anchorBody = world.CreateBody(&anchorDef);

	b2PrismaticJointDef prismDef;
	prismDef.Initialize(wallBody, anchorBody, wallBody->GetPosition(), b2Vec2(0.0f, 1.0f));
	prismDef.enableLimit = true;
	float absTravel = fabsf(travelPx) * METERS_PER_PIXEL;
	prismDef.lowerTranslation = (travelPx > 0.0f) ? -absTravel : 0.0f;
	prismDef.upperTranslation = (travelPx > 0.0f) ? 0.0f : absTravel;
	prismDef.enableMotor = false;
	prismDef.maxMotorForce = 1000.0f;
	prismDef.motorSpeed = 0.0f;

	b2PrismaticJoint* joint = static_cast<b2PrismaticJoint*>(world.CreateJoint(&prismDef));

	RegisterBodyData(wallBody, BodyTag::Obstacle);

	PrismaticWallInfo info;
	info.body = wallBody;
	info.width = wallWidth;
	info.height = wallHeight;
	info.joint = joint;
	info.triggered = false;
	info.direction = travelPx > 0.0f ? 1.0f : -1.0f;
	info.textureColumn = col;
	info.textureRowTop = wallTopRow;
	info.textureRowBottom = wallBottomRow;
	prismaticWalls.push_back(info);
}

void Scenario::CreatePrismaticWalls(b2World& world) {
	std::vector<std::vector<int>> wallByCol(buildingColumns);
	std::vector<std::vector<int>> wallTargetByCol(buildingColumns);
	ScanPrismaticPattern(wallByCol, wallTargetByCol);

	for (int col = 0; col < buildingColumns; ++col) {
		if (wallByCol[col].empty() || wallTargetByCol[col].empty()) continue;
		SpawnPrismaticWall(world, col, wallByCol[col], wallTargetByCol[col]);
	}
}

void Scenario::CreateFallSensor(b2World& world, int heightTarget) {
	float left = 0.0f;
	float width = static_cast<float>(GetScreenWidth());
	float goalY = groundTopY - heightTarget * blockSize;
	float height = groundTopY - goalY;
	float centerY = goalY + height / 2.0f;
	float centerX = left + width / 2.0f;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(centerX * METERS_PER_PIXEL, centerY * METERS_PER_PIXEL);
	fallSensorBody = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((width / 2.0f) * METERS_PER_PIXEL, (height / 2.0f) * METERS_PER_PIXEL);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = true;
	fallSensorBody->CreateFixture(&fixtureDef);

	RegisterBodyData(fallSensorBody, BodyTag::FallSensor);

	fallSensorRect = { left, centerY - height / 2.0f, width, height };
}

void Scenario::OnBlockEnteredFallZone(b2Body* blockBody) {
	for (auto& block : buildingBlocks) {
		if (block->GetBody() == blockBody) {
			block->MarkDemolished();
			break;
		}
	}
}

void Scenario::TriggerPrismaticWalls() {
	for (auto& wall : prismaticWalls) {
		if (!wall.triggered) {
			wall.joint->EnableMotor(true);
			wall.joint->SetMotorSpeed(-wall.direction * 15.0f);
			wall.joint->SetMaxMotorForce(1000.0f);
			wall.triggered = true;
		}
		else {
			float currentSpeed = wall.joint->GetMotorSpeed();
			wall.joint->SetMotorSpeed(-currentSpeed);
		}
	}
}

int Scenario::ComputeBlockLevel(const BuildingBlock& block) const {
	b2Body* body = block.GetBody();
	float bottomY = body->GetPosition().y * PIXELS_PER_METER + blockSize / 2.0f;
	int level = static_cast<int>(std::lround((groundTopY - bottomY) / blockSize));
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
	float margin = blockSize * BLOCK_FOOTPRINT_MARGIN_RATIO;
	float left = xInitialBlock - margin;
	float right = xInitialBlock + buildingColumns * blockSize+ margin;
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

	for (auto& obstacle : obstacles) {
		obstacle->Render(renderer);
	}

	for (auto& wall : prismaticWalls) {
		b2Vec2 pos = wall.body->GetPosition();
		float x = pos.x * PIXELS_PER_METER - wall.width / 2.0f;
		float y = pos.y * PIXELS_PER_METER - wall.height / 2.0f;

		if (blockTexture.id > 0 && config.textureCols > 0 && config.textureRows > 0) {
			float tileW = blockTexture.width / static_cast<float>(config.textureCols);
			float tileH = blockTexture.height / static_cast<float>(config.textureRows);
			Rectangle src = { wall.textureColumn * tileW, wall.textureRowTop * tileH,
				tileW, (wall.textureRowBottom - wall.textureRowTop + 1) * tileH };
			Rectangle dst = { x, y, wall.width, wall.height };
			DrawTexturePro(blockTexture, src, dst, { 0, 0 }, 0.0f, WHITE);
		}
		else {
			renderer.DrawRect(static_cast<int>(x), static_cast<int>(y),
				static_cast<int>(wall.width), static_cast<int>(wall.height), COLOR_WALL);
		}
		renderer.DrawRectLines(static_cast<int>(x), static_cast<int>(y),
			static_cast<int>(wall.width), static_cast<int>(wall.height), BLACK);
	}

	if (buildingHeightTarget > 0 && context.state != GameState::MainMenu) {
		Rectangle window = GetCountingWindow();
		float goalY = groundTopY - buildingHeightTarget * blockSize;
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
