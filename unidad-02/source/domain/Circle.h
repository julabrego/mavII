#pragma once
#include "raylib.h"
#include "Entity.h"

class Renderer;

class Circle : Entity
{
public:
	Circle(float x, float y, float radius, Color color, float borderThickness = 0.0f, Color borderColor = BLACK);
	void Update(float deltaTime, Renderer& renderer) override;

	void SetPosition(float x, float y);

	Vector2 position;
	float radius;
	Color color;
	float borderThickness;
	Color borderColor;
};

