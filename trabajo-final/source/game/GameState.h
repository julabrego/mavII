#pragma once

enum class GameState {
	MainMenu,
	Playing,
	Finished,
};

enum class GameFinishState {
	None,
	Won,
	Lost
};

struct GameContext {
	GameState state = GameState::MainMenu;
	GameFinishState finishState = GameFinishState::None;
	bool debugMode = false;
};