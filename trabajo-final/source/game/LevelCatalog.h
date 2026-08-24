#pragma once

#include <vector>
#include "LevelConfig.h"

class LevelCatalog {
public:
	static const std::vector<LevelConfig>& All();
	static const LevelConfig& Get(int index);
	static int Count();
};
