#pragma once

#include <box2d.h>
#include <memory>
#include <vector>

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

    void ToggleDrawJoints() { drawJoints = !drawJoints; }

private:
    void CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    void CreateBoundaryWalls(b2World&, float screenWidth, float screenHeight);

	void CreatePlayableElements(b2World& world);

    void CreateStaticWalls(b2World& world);
	void CreateTargets(b2World& world);
	
    std::unique_ptr<RectangleEntity> ground;
	std::unique_ptr<RectangleEntity> spring;

    std::vector<std::unique_ptr<CircleEntity>> targets;

    bool drawJoints = false;
};
