#pragma once
#include "raylib.h"
#include "Entity.h"
#include "box2d.h"

class Renderer;
class b2World;
class CircleEntity;

class StickEntity : public Entity
{
public:
	StickEntity(b2World& world, float x, float y, Color color, bool isLeftStick);
	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;
	void TriggerAction();
	void Reset();

	Vector2 position;
	Color color;
	bool isLeftStick;

	std::unique_ptr<CircleEntity> axis;
	std::unique_ptr<RectangleEntity> base;
	b2RevoluteJoint* joint = nullptr;
};

