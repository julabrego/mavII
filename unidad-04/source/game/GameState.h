#pragma once

enum class GameState {
	MainMenu,
	Idle,
	Launching,
	Resolved,
	Finished,
};

struct GameContext {
	GameState state = GameState::MainMenu;
	int score = 0;

	bool touchedGround = false;
	bool isStopped = false;
};