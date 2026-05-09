#include "Scenario.h"

Scenario::Scenario(b2World& world, float screenWidth, float screenHeight)
{
    CreateWall(world, 0.0f, -20.0f, screenWidth, 20.0f);
    groundBody = CreateWall(world, screenWidth / 2.0f, screenHeight - 40.0f, screenWidth / 2.0f, 20.0f);
    CreateWall(world, -20.0f, 0.0f, 20.0f, screenHeight);
    CreateWall(world, screenWidth + 20.0f, 0.0f, 20.0f, screenHeight);
}

b2Body* Scenario::CreateWall(b2World& world, float x, float y, float halfW, float halfH)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x, y);

    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(halfW, halfH);

    body->CreateFixture(&shape, 0.0f);

    return body;
}
