#include "GameUI.h"
#include "../core/Renderer.h"
#include <string>

void GameUI::Draw(Renderer& renderer, const GameContext& context, const HudInfo& hud) {
	if (context.state == GameState::Finished) {
		renderer.DrawRect(GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 225, 800, 450, Fade(BLACK, 0.8f));
		std::string msg = "";
		if (context.finishState == GameFinishState::Won) {
			if (hud.isLastLevel) {
				msg = std::string("GANASTE!\n\n")
					+ "Disparos totales: " + std::to_string(hud.totalShotsFired) + "\n"
					+ "Reintentos totales: " + std::to_string(hud.totalRetries) + "\n\n"
					+ "ENTER - volver al menu";
			}
			else {
				msg = "Nivel " + std::to_string(hud.levelNumber) + " superado!\n\n"
					
					+ "Disparos: " + std::to_string(hud.shotsFired) + "\n"
					+ "Reintentos: " + std::to_string(hud.retries) + "\n\n"
					+ "ENTER - siguiente nivel";
			}
		}
		else if (context.finishState == GameFinishState::Lost) {
			msg = "Perdiste\nENTER - reintentar el nivel";
		}
		renderer.DrawCenteredText(msg.c_str(), 40, GetScreenHeight() / 2 - 100, WHITE);
	}
	else if (context.state == GameState::Playing) {
		std::string hudText = "Nivel " + std::to_string(hud.levelNumber) + "/" + std::to_string(hud.totalLevels)
			+ "  |  Altura: " + std::to_string(hud.currentHeight)
			+ "  |  Objetivo: " + std::to_string(hud.heightTarget) + " piso" + (hud.heightTarget > 1 ? "s" : "")
			+ "  |  Disparos: " + std::to_string(hud.shotsLeft)
			+ "  |  Reintentos: " + std::to_string(hud.retries);
		renderer.DrawText(hudText.c_str(), 10, GetScreenHeight() - 30, 26, WHITE);
	}

	if (context.debugMode) {
		renderer.DrawText("DEBUG MODE (click para reubicar al personaje) - N: siguiente nivel - B: anterior nivel", 10, 10, 20, RED);
	}
}
