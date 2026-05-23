#pragma once

#include <box2d.h>

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
    RectangleEntity* staticPlatform1;
    RectangleEntity* staticWall1;
    RectangleEntity* staticPlatform2;
    RectangleEntity* staticPlatform3;

	b2Vec2 revolutePlatform1InitialPos;
    RectangleEntity* revolutePlatform1;

    RectangleEntity* positionPlatform1;
    RectangleEntity* positionPlatform2;
    RectangleEntity* positionPlatform3;

    RectangleEntity* weldObstacle1;
    RectangleEntity* weldObstacle2;

    RectangleEntity* pulleyPlatform1;
    RectangleEntity* pulleyWall1;
	bool pulleyActivated = false;
};
