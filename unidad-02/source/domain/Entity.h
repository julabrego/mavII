#pragma once

class Renderer;
class b2Body;

class Entity {
public:
	virtual ~Entity() = default;
	virtual void Update(b2Body* phisicBody, float deltaTime, Renderer& renderer) = 0;
};
