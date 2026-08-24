#include "LevelCatalog.h"

namespace {
	LevelConfig MakeLevel(int heightTarget, int availableShots,
		std::vector<std::string> pattern, std::vector<LevelConfig::ObstacleDef> obstacles)
	{
		LevelConfig config;
		config.heightTarget = heightTarget;
		config.availableShots = availableShots;
		config.pattern = std::move(pattern);
		config.obstacles = std::move(obstacles);
		return config;
	}
}

const std::vector<LevelConfig>& LevelCatalog::All()
{
	static const std::vector<LevelConfig> levels = {

		// Level 1: classic full tower
		MakeLevel(1, 3, {
			"XX",
			"XX",
			"XX",
			"XX",
			"XX",
			"XX",
			"XX",
		}, {}),

		// Level 2: pyramid
		MakeLevel(2, 3, {
			"...XX...",
			"..XXXX..",
			".XXXXXX.",
			"XXXXXXXX",
		}, {}),

		// Level 3: twin towers with a floating platform bridging the gap
		MakeLevel(1, 4, {
			"XX....XX",
			"XX....XX",
			"XX....XX",
			"XX....XX",
		}, {
			{ 410.0f, 315.0f, 180.0f, 20.0f },
		}),
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
