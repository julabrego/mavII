#pragma once
#include "raylib.h"
#include "Entity.h"
#include "box2d.h"
#include <memory>

class Renderer;
class b2World;

class RectangleEntity : public Entity
{
public:
	static std::unique_ptr<RectangleEntity> CreateStatic(b2World& world, float x, float y, float width, float height,
		float angle, Color color, float friction = 0.0f,
		float borderThickness = 0.0f, Color borderColor = BLACK);
	
	static std::unique_ptr<RectangleEntity> CreateDynamic(b2World& world, float x, float y, float width, float height,
		float angle, Color color, 
		float density, float friction, float restitution, 
		float borderThickness = 0.0f, Color borderColor = BLACK);
	
	static std::unique_ptr<RectangleEntity> CreateSensor(b2World& world, float x, float y, float width, float height,
		float angle, Color color, 
		float borderThickness = 0.0f, Color borderColor = BLACK);

	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;
	b2Body* GetBody() const { return body; }
	Vector2 GetCenter() const { return { position.x + width / 2, position.y + height / 2 }; }

	Vector2 position;
	float width;
	float height;
	float angle;
	Color color;
	float borderThickness;
	Color borderColor;

private:
	RectangleEntity(b2World& world, float x, float y, float width, float height, float angle, Color color, b2BodyType type, float density, float friction, float restitution, float borderThickness, Color borderColor, bool isSensor);
	b2Body* body;
};
