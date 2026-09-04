#include "LevelCatalog.h"

namespace {
	LevelConfig MakeLevel(int heightTarget, int availableShots,
		std::vector<std::string> pattern, std::vector<LevelConfig::ObstacleDef> obstacles,
		const std::string& texturePath = "", int textureCols = 0, int textureRows = 0)
	{
		LevelConfig config;
		config.heightTarget = heightTarget;
		config.availableShots = availableShots;
		config.pattern = std::move(pattern);
		config.obstacles = std::move(obstacles);
		config.texturePath = texturePath;
		config.textureCols = textureCols;
		config.textureRows = textureRows;
		return config;
	}
}

const std::vector<LevelConfig>& LevelCatalog::All()
{
	static const std::vector<LevelConfig> levels = {
		// Level 1: single tower
		MakeLevel(1, 3, {
			"XX",
			"XX",
			"XX",
			"XX",
			"XX",
			"XX",
			"XX",
		}, {}, "assets/level1.png", 2, 7),

		// Level 2: pyramid
		MakeLevel(2, 3, {
			"...XX...",
			"..XXXX..",
			".XXXXXX.",
			"XXXXXXXX",
		}, {}, "assets/level2.png", 8, 4),

		// Level 3: two towers + obstacle
		MakeLevel(1, 4, {
			"XX....XX",
			"XX....XX",
			"XX....XX",
			"XX....XX",
		}, {
			{ 410.0f, 315.0f, 180.0f, 20.0f },
		}, "assets/level3.png", 8, 4),

		// Level 4: single towers protected by vertical prismatic wall
		MakeLevel(1, 4, {
			"P..XX",
			"...XX",
			"...XX",
			"|..XX",
			"|..XX",
			"|..XX",
		}, {
		}, "assets/level4.png", 5, 6),

		// Level 5: single towers protected by vertical prismatic wall
		MakeLevel(1, 4, {
			"|..XX",
			"|..XX",
			"|..XX",
			"...XX",
			"...XX",
			"P..XX",
		}, {
		}, "assets/level5.png", 5, 6),
	};

	return levels;
}

const LevelConfig& LevelCatalog::Get(int index)
{
	return All()[index];
}

int LevelCatalog::Count()
{
	return static_cast<int>(All().size());
}
