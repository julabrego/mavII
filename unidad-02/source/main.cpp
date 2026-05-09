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

int main(void)
{
	const int screenWidth = 1000;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "MAVI II - Bienvenida");
	SetTargetFPS(60);

	Game game(screenWidth, screenHeight);
	game.Run();

	CloseWindow();
	return 0;
}

