#pragma once
#include "raylib.h"
#include "Entity.h"
#include "box2d.h"

class Renderer;
class b2World;

class RectangleEntity : public Entity
{
public:
	RectangleEntity(b2World& world, float x, float y, float width, float height, Color color, float borderThickness = 0.0f, Color borderColor = BLACK);
	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

	Vector2 position;
	float width;
	float height;
	Color color;
	float borderThickness;
	Color borderColor;

private:
	b2Body* body;
};


