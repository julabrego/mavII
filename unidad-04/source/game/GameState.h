#pragma once

enum class GameState {
	MainMenu,
	Idle,
	Launching,
	Resolved,
	Finished,
};

struct GameContext {
	// TODO: implement main menu
	GameState state = GameState::Idle;
	int score = 0;
	int lives = 3;
	float timer = 60.0f * 2; // 2 minutes

	bool hitTarget = false;
	bool outOfBounds = false;
};