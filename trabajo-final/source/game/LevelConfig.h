#pragma once

#include <string>
#include <vector>

struct LevelConfig {
	struct ObstacleDef {
		float x, y, w, h;
	};

	float blockSize = 45.0f;
	float groundTopY = 540.0f;
	float buildingCenterX = 0.0f;
	int heightTarget = 1;
	int availableShots = 3;
	float cannonX = 30.0f;
	float cannonY = 480.0f;
};
