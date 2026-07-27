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

class Scenario {
public:
    Scenario(b2World& world, GameContext& gameContext, float screenWidth, float screenHeight);
    ~Scenario();
    void Update(float deltaTime);
    void Render(Renderer& renderer);

private:
    b2Body* CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    void CreateBoundaryWalls(b2World&, float screenWidth, float screenHeight);
    void CreateGroundsAndWalls(b2World& world);
	void CreateBuildingBlocks(b2World& world);
    
    void RegisterBodyData(b2Body* body, BodyTag tag, void* entity = nullptr);

	GameContext& context;
	
    std::unique_ptr<RectangleEntity> ground;

	std::vector<std::unique_ptr<RectangleEntity>> buildingBlocks;

    std::vector<std::unique_ptr<BodyData>> bodyDataRegistry;
};
