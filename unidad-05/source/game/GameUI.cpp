#include "GameUI.h"
#include "../core/Renderer.h"
#include <string>

std::string mainMenuText = R"(--- SUPER MAYRO BOSS ---

ENTER - Comenzar

Instrucciones:
FLECHAS - Moverse
ESPACIO - Saltar

D - Modo debug)";

void GameUI::Draw(Renderer& renderer, const GameContext& context) {
	if (context.state == GameState::MainMenu) {
		DrawRectangle(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
		renderer.DrawCenteredText(
			mainMenuText.c_str(), 30, GetScreenHeight() / 2 - 150, WHITE);
	}
	else if (context.state == GameState::Finished) {
		DrawRectangle(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
		std::string msg = "";
		if (context.finishState == GameFinishState::Won) {
			msg = "Ganaste!\n\ENTER - volver a jugar";
		}
		else if (context.finishState == GameFinishState::Lost) {
			msg = "Perdiste\n\ENTER - volver a jugar";
		}
		renderer.DrawCenteredText(msg.c_str(), 40, GetScreenHeight() / 2 - 100, WHITE);
	}

	if (context.debugMode) {
		DrawText("DEBUG MODE (click para reubicar al personaje)", 10, 10, 20, RED);
	}
}
