#pragma once
enum class BodyTag 
{
	None,
	Player,
	PlayerGroundSensor,
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
