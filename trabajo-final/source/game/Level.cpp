#include "Level.h"
#include "../domain/Scenario.h"
#include "../core/GameplayConstants.h"

Level::Level(Scenario& scenario)
	: scenario(scenario)
{
}

void Level::Reset()
{
	shotsLeft = AVAILABLE_SHOTS;
	calmTimer = 0.0f;
	sinceLastShotTimer = 0.0f;
}

bool Level::TryConsumeShot()
{
	if (shotsLeft <= 0) {
		return false;
	}

	--shotsLeft;
	calmTimer = 0.0f;
	sinceLastShotTimer = 0.0f;
	return true;
}

GameFinishState Level::Evaluate(float deltaTime, bool worldSettled)
{
	bool heightTargetReached = scenario.GetCurrentHeight() <= buildingHeightTarget;
	bool allFallenBlocksLanded = scenario.HaveAllFallenBlocksLanded();

	if (heightTargetReached && allFallenBlocksLanded) {
		return GameFinishState::Won;
	}

	if (shotsLeft > 0) {
		return GameFinishState::None;
	}

	sinceLastShotTimer += deltaTime;

	if (worldSettled) {
		calmTimer += deltaTime;
	}
	else {
		calmTimer = 0.0f;
	}

	if (calmTimer >= SETTLE_TIME || sinceLastShotTimer >= SETTLE_HARD_TIMEOUT) {
		return GameFinishState::Lost;
	}

	return GameFinishState::None;
}
