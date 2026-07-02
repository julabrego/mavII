#pragma once

class Renderer;

class Entity {
public:
	virtual ~Entity() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Renderer& renderer) = 0;
};
