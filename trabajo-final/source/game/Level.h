#pragma once

#include "GameState.h"
#include "../core/GameplayConstants.h"

class Scenario;

class Level
{
public:
	Level(Scenario& scenario);

	void Reset();
	bool TryConsumeShot();

	int GetShotsLeft() const { return shotsLeft; }
	int GetBuildingHeightTarget() const { return buildingHeightTarget; }

	GameFinishState Evaluate(float deltaTime, bool worldSettled);

private:
	Scenario& scenario;

	int buildingHeightTarget = BUILDING_HEIGHT_TARGET;
	int shotsLeft = AVAILABLE_SHOTS;

	float calmTimer = 0.0f;
	float sinceLastShotTimer = 0.0f;
};
