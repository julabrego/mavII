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
#include <string>

const float GRAVITY = 9.8f;
const float SPRING_STRENGTH = -17.5f;
const float IMPULSE_STRENGTH = 30.0f;

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
	if (IsKeyPressed(KEY_SPACE)) {
		if(context.state == GameState::MainMenu || context.state == GameState::Finished) {
			RestartGame();
		} 
		else if (context.state == GameState::Idle) {
			circleEntity->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(IMPULSE_STRENGTH, 0.0f), true);
			context.state = GameState::Launching;
		}
	}

}

void Game::RestartGame()
{
	circleEntity->GetBody()->SetTransform({ initialBallPosition.x * METERS_PER_PIXEL, initialBallPosition.y * METERS_PER_PIXEL }, 0.0f);
	circleEntity->GetBody()->SetLinearVelocity({ 0.0f, 1.0f });
	context.score = 0;
	context.touchedGround = false;
	context.state = GameState::Idle;
}


void Game::Update(float deltaTime)
{
	scenario->Update(deltaTime);
	circleEntity->Update(deltaTime);
	auto* circleBody = circleEntity->GetBody();
	auto& contactListener = physicsWorld->GetContactListener();
	b2Vec2 circleVelocity = circleBody->GetLinearVelocity();

	if (contactListener.playerVsSpringContact) {
		circleBody->SetLinearVelocity(b2Vec2(circleVelocity.x, SPRING_STRENGTH));
	}

	if (contactListener.playerVsGroundContact) {
		circleBody->SetLinearVelocity(b2Vec2(circleVelocity.x * 0.95f, circleVelocity.y));
	}

	if (contactListener.playerVsTargetContact) {
		context.score += 100;
	}
	
	bool isCircleOutOfBounds = circleBody->GetPosition().x / METERS_PER_PIXEL > GetScreenWidth();
	float stoppedVelocityThreshold = 0.05f;
	bool isBallStopped = fabs(circleVelocity.x) < stoppedVelocityThreshold && fabs(circleVelocity.y) < stoppedVelocityThreshold;

	if (isCircleOutOfBounds || (context.state == GameState::Launching && isBallStopped)) {
		context.state = GameState::Finished;
	}
}

void Game::Draw()
{
	renderer->Begin();

	scenario->Render(*renderer);
	circleEntity->Render(*renderer);

	ui.Draw(*renderer, context);

	renderer->End();
}