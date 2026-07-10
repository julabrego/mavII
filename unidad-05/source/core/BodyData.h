#pragma once
enum class BodyTag 
{
	None,
	Player,
	Enemy,
	RotablePlatformTriggerArea,
	RotablePlatformTriggerer,
	Target,
	Wall,
	Ground,
	Box,
};

struct BodyData
{
	BodyTag tag;
	void* entity = nullptr;
};
