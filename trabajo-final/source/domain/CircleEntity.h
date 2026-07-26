#pragma once
#include "raylib.h"
#include "Entity.h"
#include "box2d.h"
#include <memory>

class Renderer;
class b2World;

class CircleEntity : public Entity
{
public:
	static std::unique_ptr<CircleEntity> CreateStatic(b2World& world, float x, float y, float radius, Color color,
		float borderThickness = 0.0f, Color borderColor = BLACK);

	static std::unique_ptr<CircleEntity> CreateDynamic(b2World& world, float x, float y, float radius, Color color,
		float density, float friction, float restitution,
		float borderThickness = 0.0f, Color borderColor = BLACK);

	static std::unique_ptr<CircleEntity> CreateSensor(b2World& world, float x, float y, float radius, Color color,
		float borderThickness = 0.0f, Color borderColor = BLACK);

	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

	b2Body* GetBody() const { return body; }

	Vector2 position;
	float radius;
	float angle = 0.0f;
	Color color;
	float borderThickness;
	Color borderColor;

private:
	CircleEntity(b2World& world, float x, float y, float radius, Color color, b2BodyType type,
		float density, float friction, float restitution, bool isSensor,
		float borderThickness, Color borderColor);
	b2Body* body;
};

