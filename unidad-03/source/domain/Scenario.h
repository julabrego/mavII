#pragma once

#include <box2d.h>
#include <memory>

class Renderer;
class RectangleEntity;

class Scenario {
public:
    Scenario(b2World& world, float screenWidth, float screenHeight);
    void Update(float deltaTime);
    void Render(Renderer& renderer);

    void InteractWithPlatform();

private:
    static void CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    std::unique_ptr<RectangleEntity> staticPlatform1;
    std::unique_ptr<RectangleEntity> staticWall1;
    std::unique_ptr<RectangleEntity> staticPlatform2;
    std::unique_ptr<RectangleEntity> staticPlatform3;

    b2Vec2 revolutePlatform1InitialPos;
    std::unique_ptr<RectangleEntity> revolutePlatform1;

    std::unique_ptr<RectangleEntity> positionPlatform1;
    std::unique_ptr<RectangleEntity> positionPlatform2;
    std::unique_ptr<RectangleEntity> positionPlatform3;

    std::unique_ptr<RectangleEntity> weldObstacle1;
    std::unique_ptr<RectangleEntity> weldObstacle2;

    std::unique_ptr<RectangleEntity> pulleyPlatform1;
    std::unique_ptr<RectangleEntity> pulleyWall1;
	bool pulleyActivated = false;
};
