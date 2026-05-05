#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <memory>

class Circle;
// class Rectangle;
// class Line;

class Renderer
{
public:
	Renderer(Color clearColor);
	~Renderer();

	void Begin();
	void End();

	void Draw(const Circle& circle);
	//void Draw(const Rectangle& rectangle);
	//void Draw(const Line& line);

private:
	std::unique_ptr<Color> clearColor;
};

#endif