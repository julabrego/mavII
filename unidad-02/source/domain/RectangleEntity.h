#pragma once
#include "raylib.h"
#include "Entity.h"
#include "box2d.h"

class Renderer;

class RectangleEntity : public Entity
{
public:
	RectangleEntity(float x, float y, float width, float height, Color color, float borderThickness = 0.0f, Color borderColor = BLACK);
	void Update(b2Body* phisicBody, float deltaTime, Renderer& renderer) override;
	void Render(Renderer& renderer) override;

	Vector2 position;
	float width;
	float height;
	Color color;
	float borderThickness;
	Color borderColor;
};


