#include "MainMenuScene.h"
#include "Game.h"
#include "GameplayScene.h"
#include "../core/Renderer.h"
#include "raylib.h"

static const char* MENU_TEXT = R"(--- DEMOLICION ---

ENTER - Comenzar

Instrucciones:
ARRIBA - ABAJO: Moverse
ESPACIO - Disparar
S - Retraer cadena

D - Modo debug
R - Reiniciar nivel)";

MainMenuScene::MainMenuScene(Game& game)
	: game(game)
	, context(game.GetContext())
{
	context.state = GameState::MainMenu;
}

void MainMenuScene::HandleInput()
{
	if (IsKeyPressed(KEY_ENTER)) {
		game.SwitchScene(std::make_unique<GameplayScene>(game, 0));
	}
}

void MainMenuScene::Update(float)
{
}

void MainMenuScene::Draw(Renderer& renderer)
{
	renderer.DrawRect(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
	renderer.DrawCenteredText(MENU_TEXT, 30, GetScreenHeight() / 2 - 150, WHITE);
}
