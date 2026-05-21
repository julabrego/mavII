#include "Scenario.h"
#include "RectangleEntity.h"

Scenario::Scenario(b2World& world, float screenWidth, float screenHeight)
{
    CreateWall(world, 0.0f, -20.0f, screenWidth, 20.0f);
    CreateWall(world, -20.0f, 0.0f, 20.0f, screenHeight);
    CreateWall(world, screenWidth + 20.0f, 0.0f, 20.0f, screenHeight);

    staticPlatform1 = new RectangleEntity(world, 0.0f, 128.0f, 185.0f, 30.0f, 0.0f, DARKGREEN);
    staticWall1 = new RectangleEntity(world, 480.0f, 0.0f, 30.0f, 125.0f, 0.0f, DARKGREEN);
    staticPlatform2 = new RectangleEntity(world, 0.0f, 330.0f, 390.0f, 30.0f, 0.0f, DARKGREEN);
    staticPlatform3 = new RectangleEntity(world, 813.0f, 545.0f, 150.0f, 30.0f, 0.0f, DARKGREEN);
    
    revolutePlatform1 = new RectangleEntity(world, 179.0f, 128.0f, 300.0f, 30.0f, 0.0f, GREEN);

    weltObstacle1 = new RectangleEntity(world, 400.0f, 330.0f, 200.0f, 15.0f, 0.0f, RED);
    weltObstacle2 = new RectangleEntity(world, 400.0f, 330.0f, 200.0f, 15.0f, 90.0f, RED);

	pulleyPlatform1 = new RectangleEntity(world, 600.0f, 330.0f, 150.0f, 30.0f, 0.0f, DARKGREEN);
	pulleyWall1 = new RectangleEntity(world, 760.0f, 250.0f, 30.0f, 300.0f, 0.0f, DARKGREEN);
}

void Scenario::CreateWall(b2World& world, float x, float y, float halfW, float halfH)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x, y);

    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(halfW, halfH);

    body->CreateFixture(&shape, 0.0f);
}

void Scenario::Render(Renderer& renderer)
{
    staticPlatform1->Render(renderer);
    staticWall1->Render(renderer);
    staticPlatform2->Render(renderer);
    staticPlatform3->Render(renderer);

    revolutePlatform1->Render(renderer);
    weltObstacle1->Render(renderer);
    weltObstacle2->Render(renderer);
	pulleyPlatform1->Render(renderer);
	pulleyWall1->Render(renderer);
}
