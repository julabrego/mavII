#pragma once

#include <box2d.h>

class Renderer;
class RectangleEntity;
class b2Vec2;

class Scenario {
public:
    Scenario(b2World& world, float screenWidth, float screenHeight);
    void Render(Renderer& renderer);

private:
    static void CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    RectangleEntity* staticPlatform1;
    RectangleEntity* staticWall1;
    RectangleEntity* staticPlatform2;
    RectangleEntity* staticPlatform3;
    
    RectangleEntity* revolutePlatform1;
	b2Vec2 revolutePlatform1JointPosition;
    RectangleEntity* weltObstacle1;
    RectangleEntity* weltObstacle2;
    RectangleEntity* pulleyPlatform1;
    RectangleEntity* pulleyWall1;
};
