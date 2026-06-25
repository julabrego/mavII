#pragma once
enum class BodyTag 
{
	None,
	Player,
	Spring,
	Target,
	Wall,
	Ground,
};

struct BodyData
{
	BodyTag tag;
};
