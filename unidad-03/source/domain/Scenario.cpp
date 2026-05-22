#include "Scenario.h"
#include "RectangleEntity.h"
#include "raylib.h"

Scenario::Scenario(b2World& world, float screenWidth, float screenHeight)
{
    CreateWall(world, 0.0f, -20.0f, screenWidth, 20.0f);
    CreateWall(world, -20.0f, 0.0f, 20.0f, screenHeight);
    CreateWall(world, screenWidth + 20.0f, 0.0f, 20.0f, screenHeight);

    staticPlatform1 = RectangleEntity::CreateStatic(world, 0.0f, 128.0f, 185.0f, 30.0f, 0.0f, DARKGREEN);
    staticWall1 = RectangleEntity::CreateStatic(world, 480.0f, 0.0f, 30.0f, 125.0f, 0.0f, DARKGREEN);
    staticPlatform2 = RectangleEntity::CreateStatic(world, 0.0f, 330.0f, 390.0f, 30.0f, 0.0f, DARKGREEN);
    staticPlatform3 = RectangleEntity::CreateStatic(world, 813.0f, 545.0f, 150.0f, 30.0f, 0.0f, DARKGREEN);
    
    revolutePlatform1 = RectangleEntity::CreateDynamic(world, 179.0f, 128.0f, 300.0f, 30.0f, 0.0f, GREEN, 1.0f, 0.5f, 0.0f);
    b2RevoluteJointDef revolutePlatform1JointDef;
    revolutePlatform1InitialPos = b2Vec2(revolutePlatform1->GetCenter().x - revolutePlatform1->width / 2, revolutePlatform1->GetCenter().y);
    revolutePlatform1JointDef.Initialize(staticPlatform1->GetBody(), revolutePlatform1->GetBody(), revolutePlatform1InitialPos);
	b2RevoluteJoint* revolutePlatform1Joint = (b2RevoluteJoint*)world.CreateJoint(&revolutePlatform1JointDef);

    positionPlatform1 = RectangleEntity::CreateDynamic(world, 0.0f, 90.0f, 50.0f, 15.0f, 0.0f, RED, 1.0f, 0.0f, 0.0f);
	positionPlatform1->GetBody()->SetLinearDamping(2.5f); // Slows down platform movement over time

	b2PrismaticJointDef positionPlatform1PrismaticDef;
	positionPlatform1PrismaticDef.Initialize(staticPlatform1->GetBody(), positionPlatform1->GetBody(), b2Vec2({ positionPlatform1->GetCenter().x, positionPlatform1->GetCenter().y }), b2Vec2(1.0f, 0.0f));
	positionPlatform1PrismaticDef.enableLimit = true;
	positionPlatform1PrismaticDef.lowerTranslation = 0.0f;
	positionPlatform1PrismaticDef.upperTranslation = 500.0f;
	b2PrismaticJoint* positionPlatform1PrismaticJoint = (b2PrismaticJoint*)world.CreateJoint(&positionPlatform1PrismaticDef);

    positionPlatform2 = RectangleEntity::CreateDynamic(world, 400.0f, 158.0f, 50.0f, 15.0f, 0.0f, RED, 1.0f, 0.5f, 0.0f);

	b2PrismaticJointDef positionPlatform2PrismaticDef;
	positionPlatform2PrismaticDef.Initialize(staticPlatform1->GetBody(), positionPlatform2->GetBody(), b2Vec2({ positionPlatform2->GetCenter().x, positionPlatform2->GetCenter().y }), b2Vec2(1.0f, 0.0f));
	b2PrismaticJoint* positionPlatform2PrismaticJoint = (b2PrismaticJoint*)world.CreateJoint(&positionPlatform2PrismaticDef);
	
    b2DistanceJointDef positionPlatform1JointDef;
	b2Vec2 positionPlatform1Anchor = b2Vec2(positionPlatform1->GetCenter().x, positionPlatform1->GetCenter().y);
	b2Vec2 positionPlatform2Anchor = b2Vec2(positionPlatform2->GetCenter().x, positionPlatform2->GetCenter().y);
	positionPlatform1JointDef.Initialize(positionPlatform1->GetBody(), positionPlatform2->GetBody(), positionPlatform1Anchor, positionPlatform2Anchor);
	positionPlatform1JointDef.length = 1.0f;
	positionPlatform1JointDef.stiffness = 0.5f;
	positionPlatform1JointDef.damping = 0.5f;
	b2DistanceJoint* positionPlatform1Joint = (b2DistanceJoint*)world.CreateJoint(&positionPlatform1JointDef);
	// Dijubar l�neas entre plataformas

    weldObstacle1 = RectangleEntity::CreateDynamic(world, 400.0f, 330.0f, 200.0f, 15.0f, 0.0f, ORANGE, 1.0f, 0.5f, 0.0f);
    weldObstacle2 = RectangleEntity::CreateDynamic(world, 400.0f, 330.0f, 200.0f, 15.0f, 90.0f, BROWN, 1.0f, 0.5f, 0.0f);
	
	b2RevoluteJointDef weldObstacle1RevoluteDef;
	weldObstacle1RevoluteDef.Initialize(staticPlatform2->GetBody(), weldObstacle1->GetBody(), b2Vec2({ weldObstacle1->GetCenter().x, weldObstacle1->GetCenter().y }));
	b2RevoluteJoint* weldObstacle1RevoluteJoint = (b2RevoluteJoint*)world.CreateJoint(&weldObstacle1RevoluteDef);

	weldObstacle1->GetBody()->SetAngularDamping(5.0f); // Slows down rotation over time
	b2WeldJointDef weldObstaclesJointDef;
	weldObstaclesJointDef.Initialize(weldObstacle1->GetBody(), weldObstacle2->GetBody(), b2Vec2({ weldObstacle1->GetCenter().x, weldObstacle1->GetCenter().y }));
	b2WeldJoint* weldObstaclesJoint = (b2WeldJoint*)world.CreateJoint(&weldObstaclesJointDef);
    // Dibujar joint

	pulleyPlatform1 = RectangleEntity::CreateDynamic(world, 620.0f, 330.0f, 150.0f, 30.0f, 0.0f, YELLOW, 1.0f, 10.5f, 0.0f);
	pulleyWall1 = RectangleEntity::CreateDynamic(world, 760.0f, 250.0f, 30.0f, 300.0f, 0.0f, YELLOW, 0.5f, 0.5f, 0.0f);

	b2Vec2 pulleyWheel = b2Vec2((pulleyPlatform1->GetCenter().x + pulleyWall1->GetCenter().x) / 2.0f, 100.0f);
	b2PulleyJointDef pulleyPlatformWallJointDef;
	pulleyPlatformWallJointDef.Initialize(pulleyPlatform1->GetBody(), pulleyWall1->GetBody(),
		pulleyWheel, pulleyWheel,
		b2Vec2(pulleyPlatform1->GetCenter().x, pulleyPlatform1->GetCenter().y),
		b2Vec2(pulleyWall1->GetCenter().x, pulleyWall1->GetCenter().y),
		1.0f);
	b2PulleyJoint* pulleyPlatformWallJoint = (b2PulleyJoint*)world.CreateJoint(&pulleyPlatformWallJointDef);

	b2PrismaticJointDef pulleyPlatformPrismaticDef;
	pulleyPlatformPrismaticDef.Initialize(staticPlatform2->GetBody(), pulleyPlatform1->GetBody(),
		{ pulleyPlatform1->GetCenter().x, pulleyPlatform1->GetCenter().y }, b2Vec2(0.0f, 1.0f));
	world.CreateJoint(&pulleyPlatformPrismaticDef);

	b2PrismaticJointDef pulleyWallPrismaticDef;
	pulleyWallPrismaticDef.Initialize(staticPlatform2->GetBody(), pulleyWall1->GetBody(),
		{ pulleyWall1->GetCenter().x, pulleyWall1->GetCenter().y }, b2Vec2(0.0f, 1.0f));
	world.CreateJoint(&pulleyWallPrismaticDef);
}

void Scenario::InteractWithPlatform()
{
	// Apply impulse on positionPlatform1 to move it to the right
    b2Vec2 impulse = b2Vec2(50000.0f, 0.0f); 
    positionPlatform1->GetBody()->ApplyLinearImpulseToCenter(impulse, true);

	// Apply impulse on weld joint to rotate weltObstacle1 and weltObstacle2
	weldObstacle1->GetBody()->ApplyAngularImpulse(5000000.0f, true);
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

void Scenario::Update(float deltaTime)
{
    revolutePlatform1->Update(deltaTime);
    positionPlatform1->Update(deltaTime);
    positionPlatform2->Update(deltaTime);

	weldObstacle1->Update(deltaTime);
	weldObstacle2->Update(deltaTime);

	pulleyPlatform1->Update(deltaTime);
	pulleyWall1->Update(deltaTime);
}

void Scenario::Render(Renderer& renderer)
{
    staticPlatform1->Render(renderer);
    staticWall1->Render(renderer);
    staticPlatform2->Render(renderer);
    staticPlatform3->Render(renderer);

    revolutePlatform1->Render(renderer);
    // Draw joint position
    DrawCircleV(Vector2({ revolutePlatform1InitialPos.x, revolutePlatform1InitialPos.y }), 6.0f, RED);

    positionPlatform1->Render(renderer);
    positionPlatform2->Render(renderer);

    weldObstacle1->Render(renderer);
    weldObstacle2->Render(renderer);

	pulleyPlatform1->Render(renderer);
	pulleyWall1->Render(renderer);

	b2Vec2 pw = b2Vec2((pulleyPlatform1->GetCenter().x + pulleyWall1->GetCenter().x) / 2.0f, 100.0f);
	DrawCircleV(Vector2({ pw.x, pw.y }), 6.0f, RED);
}
