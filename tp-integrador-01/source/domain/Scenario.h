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
    void Update(float deltaTime);
    void Render(Renderer& renderer);

	void TriggerSticksAction();
	void PullPlunger();
    void ReleasePlunger();

	void ResetSticks();
    void ToggleDrawJoints() { drawJoints = !drawJoints; }

private:
    void CreateWall(b2World& world, float x, float y, float halfW, float halfH);
    void CreateBoundaryWalls(b2World&, float screenWidth, float screenHeight);
	void CreateBridge(b2World& world, float startX, float startY, int nodeCount, float nodeSpacing);

	void CreatePlayableElements(b2World& world);

    void CreateStaticWalls(b2World& world);
	
    std::unique_ptr<RectangleEntity> dividerWall;
    std::unique_ptr<RectangleEntity> wallLeft;
    std::unique_ptr<RectangleEntity> wallRight;
    std::unique_ptr<RectangleEntity> diagonalTopLeft;
    std::unique_ptr<RectangleEntity> diagonalTopRight;
    std::unique_ptr<RectangleEntity> roof;

    std::vector<std::unique_ptr<CircleEntity>> bridgeNodes;

    std::unique_ptr<StickEntity> stickLeft;
	std::unique_ptr<StickEntity> stickRight;
    
    std::unique_ptr<RectangleEntity> plunger;
    std::unique_ptr<RectangleEntity> plungerBase;
    std::unique_ptr<RectangleEntity> plungerAxis;
    b2PrismaticJoint* plungerJoint = nullptr;
        
    bool drawJoints = false;
	bool isPlungerPulled = false;
	float plungerIdleY = 0.0f;
};
