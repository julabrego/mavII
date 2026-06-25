#include "Game.h"

#include "../core/PhysicsWorld.h"
#include "box2d.h"
#include "raylib.h"
#include "../core/Renderer.h"
#include "../domain/CircleEntity.h"
#include "../domain/RectangleEntity.h"
#include "../domain/Scenario.h"
#include "../core/Colors.h"
#include "../core/PhysicsConstants.h"
#include "../core/BodyData.h"
#include "../core/ContactListener.h"

const float GRAVITY = 9.8f;
const float IMPULSE_STRENGTH = -17.5f;

Game::Game(int screenWidth, int screenHeight)
	: physicsWorld(std::make_unique<PhysicsWorld>(GRAVITY))
	, renderer(std::make_unique<Renderer>(COLOR_SKY))
{
	b2World& world = *physicsWorld->GetWorld();

	scenario = std::make_unique<Scenario>(world, screenWidth, screenHeight);

	circleEntity = std::make_unique<CircleEntity>(world, initialBallPosition.x, initialBallPosition.y, 25.0f, COLOR_BALL, 1.0f, 0.5f, 0.5f, 2.0f);
	BodyData* playerData = new BodyData({ BodyTag::Player });
	circleEntity->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerData);
}

Game::~Game()
{
}

void Game::Run()
{
	while (!WindowShouldClose())
	{
		HandleInput();

		float deltaTime = GetFrameTime();
		physicsWorld->Update(deltaTime);
		Update(deltaTime);
		Draw();
	}
}

void Game::HandleInput()
{
	if (IsKeyPressed(KEY_V)) {
		scenario->ToggleDrawJoints();
	}

}



void Game::Update(float deltaTime)
{
	scenario->Update(deltaTime);
	circleEntity->Update(deltaTime);

	if(physicsWorld->GetContactListener().playerVsSpringContact) {
		//circleEntity->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0f, IMPULSE_STRENGTH), true);
		circleEntity->GetBody()->SetLinearVelocity(b2Vec2(0.0f, IMPULSE_STRENGTH));
	}
}

void Game::Draw()
{
	renderer->Begin();

	scenario->Render(*renderer);
	circleEntity->Render(*renderer);

	DrawUI();

	renderer->End();
}

void Game::DrawUI() {
	//DrawRectangle(10, 116, 265, 450, Fade(BLACK, 1.0f));

	DrawText(R"(02:00 | Score: 1000)", 20, 570, 20, BLACK);
}