#pragma once
enum class BodyTag 
{
	None,
	Player,
	PlayerGroundSensor,
	Target,
	Wall,
	Ground,
};

struct BodyData
{
	BodyTag tag;
	void* entity = nullptr;
};
