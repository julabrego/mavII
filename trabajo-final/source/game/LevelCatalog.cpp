#include "LevelCatalog.h"

namespace {
	LevelConfig MakeLevel(int heightTarget, int availableShots,
		std::vector<std::string> pattern, std::vector<LevelConfig::ObstacleDef> obstacles)
	{
		LevelConfig config;
		config.heightTarget = heightTarget;
		config.availableShots = availableShots;
		config.pattern = std::move(pattern);
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
		}, {}),
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
