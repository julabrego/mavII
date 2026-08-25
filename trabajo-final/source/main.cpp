#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>
#include "main.h"
#include "core/PhysicsWorld.h"
#include "core/Renderer.h"
#include "domain/CircleEntity.h"
#include "domain/RectangleEntity.h"
#include "domain/Scenario.h"
#include "game/Game.h"
#include "game/MainMenuScene.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

const char* WINDOW_TITLE = "MAVI II - Trabajo Final";

int main(void)
{

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Game game;
	game.SwitchScene(std::make_unique<MainMenuScene>(game));
	game.Run();

	CloseWindow();
	return 0;
}

