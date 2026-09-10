#pragma once

#include "Scene.h"
#include "GameState.h"
#include "raylib.h"

class Game;
class Renderer;

enum class MainMenuOptions {
	Main,
	Help,
	Credits,
	StartGame
};

class MainMenuScene : public Scene
{
public:
	explicit MainMenuScene(Game& game);
	~MainMenuScene();

	void HandleInput() override;
	void HandleButtonPresses();

	void Update(float deltaTime) override;
	void Draw(Renderer& renderer) override;

	void SelectOption(MainMenuOptions option) { currentOption = option; }

private:
	Game& game;
	GameContext& context;

	MainMenuOptions currentOption = MainMenuOptions::Main;
	bool isPlayButtonPressed = false;
	bool isHelpButtonPressed = false;
	bool isCreditsButtonPressed = false;
	bool isBackButtonPressed = false;

	float halfScreenWidth = GetScreenWidth() / 2.0f;
	float buttonWidth = 152.0f;
	float buttonHeight = 36.0f;
	float halfButtonWidth = buttonWidth / 2.0f;
	float containerWidth = 675.0f;
	float halfContainerWidth = containerWidth / 2.0f;
	float firstButtonY = 270.0f;
	float buttonsSpacing = 79.0f;
	float textY = 235.0f;
	float backButtonY = GetScreenHeight() - 100.0f;

	Texture2D backgroundTexture = LoadTexture("assets/background.png");
};

