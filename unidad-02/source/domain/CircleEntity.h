#pragma once
#include "raylib.h"
#include "Entity.h"
#include "box2d.h"

class Renderer;
class b2World;

class CircleEntity : public Entity
{
public:
	CircleEntity(b2World& world, float x, float y, float radius, Color color, float density, float friction, float restitution, float borderThickness = 0.0f, Color borderColor = BLACK);
	void Update(float deltaTime, Renderer& renderer) override;
	void Render(Renderer& renderer) override;

	b2Body* GetBody() const { return body; }

	Vector2 position;
	float radius;
	Color color;
	float borderThickness;
	Color borderColor;

private:
	b2Body* body;
};

