#pragma once

#include "Entity.h"
#include <memory>
#include "raylib.h"

class RectangleEntity;
class b2World;
class b2Body;

class BuildingBlock : public Entity
{
public:
	static std::unique_ptr<BuildingBlock> Create(b2World& world, int row, int column,
		float x, float y, float width, float height, Color color,
		float density, float friction, float restitution, float borderThickness = 0.0f);

	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

	b2Body* GetBody() const;
	int GetRow() const { return row; }
	int GetColumn() const { return column; }
	bool IsStanding() const;
	bool IsLanded() const { return landed; }
	void SetShadowed(bool value) { shadowed = value; }

private:
	BuildingBlock(std::unique_ptr<RectangleEntity> shape, int row, int column,
		Vector2 homeCenter, float fallTolerance, Color baseColor);

	std::unique_ptr<RectangleEntity> shape;
	int row;
	int column;
	Vector2 homeCenter;
	float fallTolerance;
	Color baseColor;
	bool demolished = false;
	bool landed = false;
	bool shadowed = false;
	float landedTimer = 0.0f;
};
