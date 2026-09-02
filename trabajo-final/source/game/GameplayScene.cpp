#include "GameplayScene.h"

#include "Game.h"
#include "../core/PhysicsWorld.h"
#include "box2d.h"
#include "raylib.h"
#include "../core/Renderer.h"
#include "../domain/RectangleEntity.h"
#include "../domain/Scenario.h"
#include "../domain/WreckingBall.h"
#include "Level.h"
#include "../core/Colors.h"
#include "../core/PhysicsConstants.h"
#include "../core/GameplayConstants.h"
#include "../core/ContactListener.h"
#include "LevelCatalog.h"
#include "MainMenuScene.h"
#include <cmath>

GameplayScene::GameplayScene(Game& game, int levelIndex)
	: game(game)
	, context(game.GetContext())
{
	LoadLevel(levelIndex);
}

GameplayScene::~GameplayScene() = default;

void GameplayScene::LoadLevel(int index)
{
	levelIndex = index;

	physicsWorld = std::make_unique<PhysicsWorld>(GRAVITY);
	b2World& world = *physicsWorld->GetWorld();

	chain.Reset();
	chain.SetWorld(world);

	context = GameContext{};

	const LevelConfig config = LevelCatalog::Get(index);

	scenario = std::make_unique<Scenario>(world, context, config);
	player = std::make_unique<PlayerCannon>(world, context, float(config.cannonX), float(config.cannonY));
	level = std::make_unique<Level>(*scenario, config);

	context.state = GameState::Playing;
}

void GameplayScene::HandleInput()
{
	if (context.state == GameState::Finished) {
		if (IsKeyPressed(KEY_ENTER)) {
			if (context.finishState == GameFinishState::Won) {
				if (levelIndex + 1 < LevelCatalog::Count()) {
					LoadLevel(levelIndex + 1);
				}
				else {
					game.SwitchScene(std::make_unique<MainMenuScene>(game));
				}
			}
			else {
				LoadLevel(levelIndex);
			}
		}
		return;
	}

	if (player) {
		player->SetAction(PlayerCannonAction::RotateLeft, IsKeyDown(KEY_LEFT));
		player->SetAction(PlayerCannonAction::RotateRight, IsKeyDown(KEY_RIGHT));
		player->SetAction(PlayerCannonAction::MoveUp, IsKeyDown(KEY_UP));
		player->SetAction(PlayerCannonAction::MoveDown, IsKeyDown(KEY_DOWN));
		player->SetAction(PlayerCannonAction::Shoot, IsKeyDown(KEY_SPACE));
		player->SetAction(PlayerCannonAction::Pull, IsKeyDown(KEY_S));
	}

	if (IsKeyPressed(KEY_D)) {
		context.debugMode = !context.debugMode;
	}
	else if (IsKeyPressed(KEY_R)) {
		LoadLevel(levelIndex);
	}
	else if (context.debugMode) {
		if (IsKeyPressed(KEY_N) && levelIndex + 1 < LevelCatalog::Count()) {
			LoadLevel(levelIndex + 1);
		}
		else if (IsKeyPressed(KEY_B) && levelIndex > 0) {
			LoadLevel(levelIndex - 1);
		}
	}

	if (context.debugMode) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			float mouseX = GetMouseX() * METERS_PER_PIXEL;
			float mouseY = GetMouseY() * METERS_PER_PIXEL;
			player->TeleportTo(mouseX, mouseY);
		}
	}
}

void GameplayScene::Update(float deltaTime)
{
	if (context.state == GameState::Playing) {
		physicsWorld->Update(deltaTime);

		ContactListener& listener = physicsWorld->GetContactListener();
		for (b2Body* blockBody : listener.fallSensorEvents) {
			scenario->OnBlockEnteredFallZone(blockBody);
		}

		for (b2Vec2& collisionPoint : listener.wreckingBallCollisionPoints) {
			float speed = chain.GetBall()->GetBody()->GetLinearVelocity().Length();
			if (speed > MIN_EMIT_SPEED) {
				Vector2 pos = { collisionPoint.x * PIXELS_PER_METER, collisionPoint.y * PIXELS_PER_METER };
				particles.Emit(pos, SPARK, 8);
			}
		}

		listener.ClearFrameEvents();
	}

	scenario->Update(deltaTime, level->GetBuildingHeightTarget());

	if (player) {
		player->Update(deltaTime);

		if (player->GetPullRequested()) {
			chain.Pull(deltaTime, player->GetBody());
		}

		if (!player->GetPullRequested() && player->GetShootRequested()) {
			bool shotConsumed = !chain.HasBall() && level->TryConsumeShot();
			if (shotConsumed) {
				scenario->TriggerPrismaticWalls();
			}
			if (chain.HasBall() || shotConsumed) {
				if (!chain.HasBall() || !chain.AtCapacity()) {
					float angleRad = player->GetBody()->GetAngle();
					float spawnX = player->GetBody()->GetPosition().x * PIXELS_PER_METER + cosf(angleRad) * LAUNCH_OFFSET;
					float spawnY = player->GetBody()->GetPosition().y * PIXELS_PER_METER + sinf(angleRad) * LAUNCH_OFFSET;

					b2Vec2 cannonPos = player->GetPosition();
					b2Vec2 headPos = chain.HasBall() && chain.LinkCount() > 0
						? chain.GetLastLinkBody()->GetPosition()
						: (chain.HasBall() ? chain.GetBall()->GetBody()->GetPosition() : cannonPos);
					float dist = b2Distance(cannonPos, headPos);

					if (!chain.HasBall() || dist > CHAIN_LINK_SPACING * METERS_PER_PIXEL) {
						chain.SpawnLink(player->GetBody(), angleRad, spawnX, spawnY);
					}
				}
			}
		}
	}

	chain.Update(deltaTime);

	chain.CleanupOffScreen(GetScreenWidth(), GetScreenHeight(), player ? player->GetBody() : nullptr);

	if (context.state == GameState::Playing) {
		GameFinishState finishState = level->Evaluate(deltaTime, physicsWorld->IsSettled());
		if (finishState != GameFinishState::None) {
			context.finishState = finishState;
			context.state = GameState::Finished;
		}
	}

	particles.Update(deltaTime, GetScreenWidth(), GetScreenHeight());

	HandlePlayerDeath();
}

void GameplayScene::Draw(Renderer& renderer)
{
	scenario->Render(renderer, level->GetBuildingHeightTarget());

	chain.Render(renderer);

	if (player) {
		player->Render(renderer);
	}

	if (context.debugMode) {
		Rectangle window = scenario->GetCountingWindow();
		renderer.DrawRectLines(static_cast<int>(window.x), static_cast<int>(window.y),
			static_cast<int>(window.width), static_cast<int>(window.height), Fade(GREEN, 0.5f));

		Rectangle sensor = scenario->GetFallSensorRect();
		renderer.DrawRect(static_cast<int>(sensor.x), static_cast<int>(sensor.y),
			static_cast<int>(sensor.width), static_cast<int>(sensor.height), Fade(RED, 0.2f));
		renderer.DrawRectLines(static_cast<int>(sensor.x), static_cast<int>(sensor.y),
			static_cast<int>(sensor.width), static_cast<int>(sensor.height), Fade(RED, 0.6f));

		chain.RenderDebug(renderer);
	}

	particles.Draw();

	HudInfo hud;
	hud.levelNumber = levelIndex + 1;
	hud.isLastLevel = (levelIndex + 1) == LevelCatalog::Count();
	hud.shotsLeft = level->GetShotsLeft();
	hud.currentHeight = scenario->GetCurrentHeight();
	hud.heightTarget = level->GetBuildingHeightTarget();
	ui.Draw(renderer, context, hud);
}

void GameplayScene::HandlePlayerDeath()
{
	if (!player) return;

	float screenHeight = GetScreenHeight();

	float playerY = player->GetBody()->GetPosition().y * PIXELS_PER_METER;
	float playerHalfHeight = 30.0f;
	if (playerY > screenHeight + playerHalfHeight && context.state == GameState::Playing) {
		player->Die();
	}

	if (player->GetState() == PlayerCannonState::Dead && context.state == GameState::Playing) {
		physicsWorld->DestroyBody(player->GetBody());
		player.reset();

		context.state = GameState::Finished;
		context.finishState = GameFinishState::Lost;
	}
}
