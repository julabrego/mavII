#pragma once

#include <box2d.h>
#include <memory>
#include <vector>
#include <raylib.h>
#include "Enemy.h"
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

	void EnablePlatformRotation();

private:
    b2Body* CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    void CreateBoundaryWalls(b2World&, float screenWidth, float screenHeight);
    void CreateGroundsAndWalls(b2World& world);
    void CreateRotablePlatform(b2World& world);
    void CreateDynamicElements(b2World& world);
    void CreateFinishSensor(b2World& world);
    void RegisterBodyData(b2Body* body, BodyTag tag, void* entity = nullptr);

	GameContext& context;
	
    std::unique_ptr<RectangleEntity> ground;
    std::unique_ptr<RectangleEntity> ground2;
    std::unique_ptr<RectangleEntity> wall1;
    std::unique_ptr<RectangleEntity> ground3;

    std::unique_ptr<RectangleEntity> rotablePlatform;
    std::unique_ptr<RectangleEntity> movableWall;
    std::unique_ptr<RectangleEntity> box1;
    
    std::unique_ptr<RectangleEntity> obstacleSensor1;

    std::unique_ptr<RectangleEntity> finishSensor;

    Texture2D finalFlagTexture = LoadTexture("../assets/flag.png");
    std::vector<std::unique_ptr<BodyData>> bodyDataRegistry;
};
