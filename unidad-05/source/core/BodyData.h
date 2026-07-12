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
	BodyData() = default;
	BodyData(BodyTag tag, void* entity = nullptr) : tag(tag), entity(entity) {}

	BodyTag tag;
	void* entity = nullptr;
};
