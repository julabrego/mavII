#pragma once
enum class BodyTag 
{
	None,
	Player,
	Enemy,
	RotablePlatformTriggerArea,
	RotablePlatformTriggerer,
	Wall,
	Ground,
	Box,
	FinishSensor
};

struct BodyData
{
	BodyTag tag;
	void* entity = nullptr;
};
