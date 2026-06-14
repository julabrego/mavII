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

    void ToggleDrawJoints() { drawJoints = !drawJoints; }

private:
    void CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    void CreateBoundaryWalls(b2World&, float screenWidth, float screenHeight);

    void CreateStaticWalls(b2World& world);
	
    std::unique_ptr<RectangleEntity> dividerWall;
    std::unique_ptr<RectangleEntity> wallLeft;
    std::unique_ptr<RectangleEntity> wallRight;
    std::unique_ptr<RectangleEntity> diagonalTopLeft;
    std::unique_ptr<RectangleEntity> diagonalTopRight;
    std::unique_ptr<RectangleEntity> roof;
        
    bool drawJoints = false;
};
