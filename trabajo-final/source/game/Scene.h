#pragma once

class Renderer; 

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void HandleInput() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(Renderer& renderer) = 0;
};


