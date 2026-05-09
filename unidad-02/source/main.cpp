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

const float GRAVITY = 9.8f;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

const char* WINDOW_TITLE = "MAVI II - Unidad 02";

int main(void)
{

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
	game.Run();

	CloseWindow();
	return 0;
}

