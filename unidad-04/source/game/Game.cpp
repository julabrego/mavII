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
	if (IsKeyPressed(KEY_V)) {
		scenario->ToggleDrawJoints();
	}

	else if (IsKeyPressed(KEY_SPACE)) {
		if(context.state == GameState::MainMenu || context.state == GameState::Finished) {
			RestartBallPosition();
			context.score = 0;
			context.touchedGround = false;
		}
		else if (context.state == GameState::Idle) {
			circleEntity->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(IMPULSE_STRENGTH, 0.0f), true);
			context.state = GameState::Launching;
		}
	}

}

void Game::RestartBallPosition()
{
	circleEntity->GetBody()->SetTransform({ initialBallPosition.x * METERS_PER_PIXEL, initialBallPosition.y * METERS_PER_PIXEL }, 0.0f);
	circleEntity->GetBody()->SetLinearVelocity({ 0.0f, 1.0f });
	context.state = GameState::Idle;
}


void Game::Update(float deltaTime)
{
	scenario->Update(deltaTime);
	circleEntity->Update(deltaTime);

	b2Vec2 vel = circleEntity->GetBody()->GetLinearVelocity();
	if (physicsWorld->GetContactListener().playerVsSpringContact) {
		circleEntity->GetBody()->SetLinearVelocity(b2Vec2(vel.x, SPRING_STRENGTH));
	}

	if (circleEntity->GetBody()->GetPosition().x / METERS_PER_PIXEL > GetScreenWidth() || context.isStopped) {
		context.state = GameState::Finished;
	}

	if (physicsWorld->GetContactListener().playerVsGroundContact) {
		b2Vec2 velocity = circleEntity->GetBody()->GetLinearVelocity();
		circleEntity->GetBody()->SetLinearVelocity(b2Vec2(velocity.x * 0.95f, velocity.y));
	}
	
	if (context.state == GameState::Launching && fabs(vel.x) < 0.05f && fabs(vel.y) < 0.05f) {
		context.state = GameState::Finished;
	}

	if (physicsWorld->GetContactListener().playerVsTargetContact) {
		context.score += 100;
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

	if (context.state == GameState::MainMenu) {
		DrawRectangle(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
		renderer->DrawCenteredText(
			("Espacio para jugar"), 40, GetScreenHeight() / 2 - 100, RED);
	}
	else if (context.state == GameState::Finished) {
		DrawRectangle(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
		renderer->DrawCenteredText(
			(R"(Game Over
Score: )" + std::to_string(context.score) + R"(
Espacio para volver a jugar)").c_str(),
			40, GetScreenHeight() / 2 - 100, RED);
	}

	DrawText(("Score: " + std::to_string(context.score)).c_str(), 20, 570, 20, BLACK);
}