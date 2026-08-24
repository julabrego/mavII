#pragma once

#include <string>
#include <vector>

struct LevelConfig {
	struct ObstacleDef {
		float x, y, w, h; // pixels, top-left corner
	};

	float blockSize = 45.0f;
	float groundTopY = 540.0f;
	float buildingCenterX = 0.0f; // 0 = horizontal screen center
	int heightTarget = 1;
	int availableShots = 3;
	float cannonX = 30.0f;
	float cannonY = 480.0f;

	// ASCII layout, top to bottom: 'X' = block, anything else = empty
	std::vector<std::string> pattern;

	std::vector<ObstacleDef> obstacles;
};
