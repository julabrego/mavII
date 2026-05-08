#pragma once
#include "raylib.h"
#include "Entity.h"
#include "box2d.h"

class Renderer;

class CircleEntity : public Entity
{
public:
	CircleEntity(float x, float y, float radius, Color color, float borderThickness = 0.0f, Color borderColor = BLACK);
	void Update(b2Body* phisicBody, float deltaTime, Renderer& renderer) override;

	Vector2 position;
	float radius;
	Color color;
	float borderThickness;
	Color borderColor;
};

