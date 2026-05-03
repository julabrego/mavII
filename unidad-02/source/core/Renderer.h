#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <memory>

struct Circle {
	Vector2 position;
	float radius;
	Color color;
	float borderThickness;
	Color borderColor;
};

class Renderer
{
public:
	Renderer(Color clearColor);
	~Renderer();

	void Begin();
	void End();

	void DrawCircle(Circle circle);

private:
	std::unique_ptr<Color> clearColor;
};

#endif