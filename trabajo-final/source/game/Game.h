#pragma once

#include <memory>
#include "GameState.h"

class Renderer;
class Scene;

class Game
{
public:
	Game();
	~Game();
	void Run();
	void SwitchScene(std::unique_ptr<Scene> scene);

	GameContext& GetContext() { return context; }
	Renderer& GetRenderer() { return *renderer; }

private:
	GameContext context;

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Scene> currentScene;

};

