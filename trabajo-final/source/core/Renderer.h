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

	void DrawSprite(Texture2D texture, Rectangle src, Rectangle dst, float rotation = 0.0f, Color tint = WHITE);
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawRectLines(int x, int y, int w, int h, Color color);
	void DrawText(const char* text, int x, int y, int fontSize, Color color);
	void DrawCenteredText(const char* text, int fontSize, int posY, Color color);

private:
	std::unique_ptr<Color> clearColor;
};

#endif