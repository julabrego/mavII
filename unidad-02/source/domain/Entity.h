#pragma once

class Renderer;

class Entity {
public:
	virtual ~Entity() = default;
	virtual void Update(float deltaTime, Renderer& renderer) = 0;
};
