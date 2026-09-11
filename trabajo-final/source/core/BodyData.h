#pragma once
enum class BodyTag 
{
	None,
	Player,
	RotablePlatformTriggerArea,
	RotablePlatformTriggerer,
	Wall,
	Ground,
	Box,
	FinishSensor,
	WreckingBall,
	ChainLink,
	BuildingBlock,
	Obstacle,
	FallSensor
};

struct BodyData
{
	BodyData() = default;
	BodyData(BodyTag tag, void* entity = nullptr) : tag(tag), entity(entity) {}

	BodyTag tag;
	void* entity = nullptr;
};
