#include "GameUI.h"
#include "../core/Renderer.h"
#include <string>

void GameUI::Draw(Renderer& renderer, const GameContext& context, const HudInfo& hud) {
	if (context.state == GameState::Finished) {
		renderer.DrawRect(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
		std::string msg = "";
		if (context.finishState == GameFinishState::Won) {
			if (hud.isLastLevel) {
				msg = "Juego completado!\nENTER - volver al menu";
			}
			else {
				msg = "Nivel " + std::to_string(hud.levelNumber) + " superado!\nENTER - siguiente nivel";
			}
		}
		else if (context.finishState == GameFinishState::Lost) {
			msg = "Perdiste\nENTER - reintentar el nivel";
		}
		renderer.DrawCenteredText(msg.c_str(), 40, GetScreenHeight() / 2 - 100, WHITE);
	}
	else if (context.state == GameState::Playing) {
		std::string hudText = "Nivel " + std::to_string(hud.levelNumber)
			+ "  Altura: " + std::to_string(hud.currentHeight)
			+ "  Meta: " + std::to_string(hud.heightTarget)
			+ "  Disparos: " + std::to_string(hud.shotsLeft);
		renderer.DrawText(hudText.c_str(), 10, 40, 24, WHITE);
	}

	if (context.debugMode) {
		renderer.DrawText("DEBUG MODE (click para reubicar al personaje) - N: siguiente nivel", 10, 10, 20, RED);
	}
}
