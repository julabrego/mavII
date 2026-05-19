#pragma once

#include <box2d.h>

class Scenario {
public:
    Scenario(b2World& world, float screenWidth, float screenHeight);

private:
    static void CreateWall(b2World& world, float x, float y, float halfW, float halfH);
};
