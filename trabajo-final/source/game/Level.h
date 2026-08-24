#pragma once

#include "GameState.h"
#include "LevelConfig.h"

class Scenario;

class Level
{
public:
	Level(Scenario& scenario, const LevelConfig& config);

	void Reset();
	bool TryConsumeShot();

	int GetShotsLeft() const { return shotsLeft; }
	int GetBuildingHeightTarget() const { return buildingHeightTarget; }

	GameFinishState Evaluate(float deltaTime, bool worldSettled);

private:
	Scenario& scenario;
	LevelConfig config;

	int buildingHeightTarget = 1;
	int shotsLeft = 3;

	float calmTimer = 0.0f;
	float sinceLastShotTimer = 0.0f;
};
