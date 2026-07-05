#pragma once

#include <box2d.h>
#include <memory>
#include <vector>
#include <raylib.h>

class Renderer;
class RectangleEntity;
class StickEntity;
class CircleEntity;

class Scenario {
public:
    Scenario(b2World& world, float screenWidth, float screenHeight);
    ~Scenario();
    void Update(float deltaTime);
    void Render(Renderer& renderer);

private:
    void CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    void CreateBoundaryWalls(b2World&, float screenWidth, float screenHeight);

    void CreateStaticWalls(b2World& world);
	
    std::unique_ptr<RectangleEntity> ground;
    std::unique_ptr<RectangleEntity> ground2;
    std::unique_ptr<RectangleEntity> wall1;
    std::unique_ptr<RectangleEntity> ground3;

    std::unique_ptr<RectangleEntity> rotablePlatform;

    Texture2D finalFlagTexture = LoadTexture("../assets/flag.png");
};
