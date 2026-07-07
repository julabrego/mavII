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
};

struct BodyData
{
	BodyTag tag;
	void* entity = nullptr;
};
