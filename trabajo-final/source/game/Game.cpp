#include "Game.h"

#include "Scene.h"
#include "../core/Renderer.h"
#include "../core/Colors.h"
#include "raylib.h"

Game::Game()
	: renderer(std::make_unique<Renderer>(COLOR_SKY))
{
}

Game::~Game() = default;

void Game::Run()
{
	while (!WindowShouldClose())
	{
		if (!currentScene) {
			continue;
		}

		currentScene->HandleInput();

		float deltaTime = GetFrameTime();
		currentScene->Update(deltaTime);

		renderer->Begin();
		currentScene->Draw(*renderer);
		renderer->End();
	}
}

void Game::SwitchScene(std::unique_ptr<Scene> scene)
{
	currentScene = std::move(scene);
}