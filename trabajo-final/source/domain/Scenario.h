#pragma once

#include <box2d.h>
#include <memory>
#include <vector>
#include <raylib.h>
#include "../game/GameState.h"
#include "../core/BodyData.h"
#include "../game/LevelConfig.h"

class Renderer;
class RectangleEntity;
class StickEntity;
class CircleEntity;
class BuildingBlock;

class Scenario {
public:
    Scenario(b2World& world, GameContext& gameContext, const LevelConfig& levelConfig);
    ~Scenario();
    void Update(float deltaTime, int buildingHeightTarget);
    void Render(Renderer& renderer, int buildingHeightTarget);

    int GetCurrentHeight() const;
    bool HaveAllFallenBlocksLanded() const;
    Rectangle GetCountingWindow() const;
    const std::vector<std::unique_ptr<BuildingBlock>>& GetBuildingBlocks() const { return buildingBlocks; }
    void TriggerPrismaticWalls();
    void OnBlockEnteredFallZone(b2Body* blockBody);
    Rectangle GetFallSensorRect() const { return fallSensorRect; }

private:
    b2Body* CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    void CreateBoundaryWalls(b2World& world);
    void CreateGround(b2World& world);

	void CreateBuildingBlocks(b2World& world);
    void CreateObstacles(b2World& world);
    void CreatePrismaticWalls(b2World& world);
    void CreateFallSensor(b2World& world, int heightTarget);
    void ScanPrismaticPattern(std::vector<std::vector<int>>& pipeByCol, std::vector<std::vector<int>>& pushByCol) const;
    void SpawnPrismaticWall(b2World& world, int col, const std::vector<int>& pipeRows, const std::vector<int>& pushRows);

    void DrawDashedHLine(Renderer& renderer, float y, float x1, float x2, Color color) const;
    int ComputeBlockLevel(const BuildingBlock& block) const;
    
    void RegisterBodyData(b2Body* body, BodyTag tag, void* entity = nullptr);

	GameContext& context;
    LevelConfig config;
	
	int buildingRows = 0;
	int buildingColumns = 0;
    float blockSize = 45.0f;

	float xInitialBlock = 0.0f;

    float groundTopY = 540.0f;

    std::unique_ptr<RectangleEntity> ground;

    Texture2D blockTexture = { 0 };
	Texture2D groundTexture = LoadTexture("assets/grass01.png");
	Texture2D backgroundTexture = LoadTexture("assets/background.png");

	std::vector<std::unique_ptr<BuildingBlock>> buildingBlocks;
    std::vector<std::unique_ptr<RectangleEntity>> obstacles;

    struct PrismaticWallInfo {
        b2Body* body = nullptr;
        float width = 0.0f;
        float height = 0.0f;
        b2PrismaticJoint* joint = nullptr;
        bool triggered = false;
        float direction = 0.0f;
        int textureColumn = 0;
        int textureRowTop = 0;
        int textureRowBottom = 0;
    };
    std::vector<PrismaticWallInfo> prismaticWalls;

    std::vector<std::unique_ptr<BodyData>> bodyDataRegistry;

    b2Body* fallSensorBody = nullptr;
    Rectangle fallSensorRect = { 0, 0, 0, 0 };
};
