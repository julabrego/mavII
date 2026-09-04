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
	float cannonX = 35.0f;
	float cannonY = 271.0f;

	std::string texturePath;
	int textureCols = 0;
	int textureRows = 0;

	std::vector<std::string> pattern;
	std::vector<ObstacleDef> obstacles;
};

