#pragma once

#include <box2d.h>
#include <memory>
#include <vector>
#include <raylib.h>
#include "../game/GameState.h"
#include "../core/BodyData.h"

class Renderer;
class RectangleEntity;
class StickEntity;
class CircleEntity;
class BuildingBlock;

class Scenario {
public:
    Scenario(b2World& world, GameContext& gameContext, float screenWidth, float screenHeight);
    ~Scenario();
    void Update(float deltaTime, int buildingHeightTarget);
    void Render(Renderer& renderer, int buildingHeightTarget);

    int GetCurrentHeight() const;
    bool HaveAllFallenBlocksLanded() const;
    Rectangle GetCountingWindow() const;
    const std::vector<std::unique_ptr<BuildingBlock>>& GetBuildingBlocks() const { return buildingBlocks; }

private:
    b2Body* CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    void CreateBoundaryWalls(b2World&, float screenWidth, float screenHeight);
    void CreateGroundsAndWalls(b2World& world);
	void CreateBuildingBlocks(b2World& world);
    void DrawDashedHLine(Renderer& renderer, float y, float x1, float x2, Color color) const;
    int ComputeBlockLevel(const BuildingBlock& block) const;
    
    void RegisterBodyData(b2Body* body, BodyTag tag, void* entity = nullptr);

	GameContext& context;
	
    std::unique_ptr<RectangleEntity> ground;

	int buildingRows = 0;
	int buildingColumns = 0;
	float blockWidth = 0.0f;
	float blockHeight = 0.0f;
	float xInitialBlock = 0.0f;
	float yInitialBlock = 0.0f;
	float groundTopY = 540.0f;

	std::vector<std::unique_ptr<BuildingBlock>> buildingBlocks;

    std::vector<std::unique_ptr<BodyData>> bodyDataRegistry;
};
