#pragma once

#include <box2d.h>

class Scenario {
public:
    Scenario(b2World& world, float screenWidth, float screenHeight);

    b2Body* GetGroundBody() const { return groundBody; }

private:
    b2Body* groundBody;

    static b2Body* CreateWall(b2World& world, float x, float y, float halfW, float halfH);
};
