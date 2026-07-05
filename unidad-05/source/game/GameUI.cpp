#include "GameUI.h"
#include "../core/Renderer.h"
#include <string>

std::string mainMenuText = R"(--- LA BOUNCING BOCH ---

ESPACIO - Comenzar

Instrucciones:
ESPACIO - Lanzar la pelota

Rebota contra los targets para sumar puntos)";

void GameUI::Draw(Renderer& renderer, const GameContext& context) {
	return;
	if (context.state == GameState::MainMenu) {
		DrawRectangle(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
		renderer.DrawCenteredText(
			mainMenuText.c_str(), 30, GetScreenHeight() / 2 - 150, WHITE);
	}
	else if (context.state == GameState::Finished) {
		DrawRectangle(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
		std::string msg = "Game Over\nScore: " + std::to_string(context.score) + "\n\nESPACIO - volver a jugar";
		renderer.DrawCenteredText(msg.c_str(), 40, GetScreenHeight() / 2 - 100, WHITE);
	}

	DrawText(("Score: " + std::to_string(context.score)).c_str(), 20, 570, 20, BLACK);
}
