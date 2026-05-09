#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <memory>

class CircleEntity;
class RectangleEntity;

class Renderer
{
public:
	Renderer(Color clearColor);
	~Renderer();

	void Begin();
	void End();

	void Draw(const CircleEntity& circle);
	void Draw(const RectangleEntity& rectangle);

	void DrawCenteredText(const char* text, int fontSize, int posY, Color color);

private:
	std::unique_ptr<Color> clearColor;
};

#endif