#include "MainMenuScene.h"
#include "Game.h"
#include "GameplayScene.h"
#include "../core/Renderer.h"
#include "raylib.h"
#include "../main.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static const char* TITLE_TEXT = R"(Godzilla pega labu)";

static const char* HELP_TEXT = R"(INSTRUCCIONES:

ARRIBA - ABAJO: Moverse
IZQUIERDA - DERECHA: Apuntar
ESPACIO - Disparar y extender cadena
S - Retraer cadena

R - Reintentar nivel)";

static const char* CREDITS_TEXT = R"(CREDITOS:

Desarrollado por Julian Ezequiel Abrego como proyecto final 
para la materia Modelos y Algoritmos de Videojuegos II
de la Tecnicatura en Diseño y Programacion de Videojuegos
de la Universidad Nacional del Litoral, Argentina

Algunos assets por Kenney (kenney.nl)
Licencia CC0)";

static const char* FOOTER_TEXT = "Julian Abrego - " GAME_VERSION;

MainMenuScene::MainMenuScene(Game& game)
	: game(game)
	, context(game.GetContext())
{
	context.state = GameState::MainMenu;
}

MainMenuScene::~MainMenuScene()
{
	if (backgroundTexture.id > 0) {
		UnloadTexture(backgroundTexture);
	}
}

void MainMenuScene::HandleInput()
{
	// TODO: no input handling required anymore :)
}

void MainMenuScene::HandleButtonPresses()
{
	if (isPlayButtonPressed) {
		currentOption = MainMenuOptions::StartGame;
	}
	else if (isHelpButtonPressed) {
		currentOption = MainMenuOptions::Help;
	}
	else if (isCreditsButtonPressed) {
		currentOption = MainMenuOptions::Credits;
	}
	else if (isBackButtonPressed) {
		currentOption = MainMenuOptions::Main;
	}
}

void MainMenuScene::Update(float)
{
	HandleButtonPresses();

	if (currentOption == MainMenuOptions::StartGame) {
		game.SwitchScene(std::make_unique<GameplayScene>(game, 0));
	}
}

void MainMenuScene::Draw(Renderer& renderer)
{
	isPlayButtonPressed = false;
	isHelpButtonPressed = false;
	isCreditsButtonPressed = false;
	isBackButtonPressed = false;

	DrawTexture(backgroundTexture, 0, 120, WHITE);

	renderer.DrawRect(halfScreenWidth - halfContainerWidth , 0, containerWidth, GetScreenHeight(), Fade(BLACK, 0.8f));
	renderer.DrawCenteredText(TITLE_TEXT, 50, 125, WHITE);
	renderer.DrawCenteredText(FOOTER_TEXT, 20, GetScreenHeight() - 30, WHITE);

	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

	if (currentOption == MainMenuOptions::Main) {
		isPlayButtonPressed = GuiButton({ halfScreenWidth - halfButtonWidth, firstButtonY, buttonWidth, buttonHeight }, "Jugar");
		isHelpButtonPressed = GuiButton({ halfScreenWidth - halfButtonWidth, firstButtonY + buttonsSpacing, buttonWidth, buttonHeight }, "Instrucciones");
		isCreditsButtonPressed = GuiButton({ halfScreenWidth - halfButtonWidth, firstButtonY + 2 * buttonsSpacing, buttonWidth, buttonHeight }, "Creditos");
	}
	else if (currentOption == MainMenuOptions::Help) {
		renderer.DrawCenteredText(HELP_TEXT, 20, textY, WHITE);
		isBackButtonPressed = GuiButton({ halfScreenWidth - halfButtonWidth, backButtonY, buttonWidth, buttonHeight }, "Volver");
	}
	else if (currentOption == MainMenuOptions::Credits) {
		renderer.DrawCenteredText(CREDITS_TEXT, 20, textY, WHITE);
		isBackButtonPressed = GuiButton({ halfScreenWidth - halfButtonWidth, backButtonY, buttonWidth, buttonHeight }, "Volver");
	}
}
