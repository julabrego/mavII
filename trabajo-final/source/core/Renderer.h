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
	void DrawSprite(Texture2D texture, Rectangle src, Rectangle dst, Vector2 origin, float rotation = 0.0f, Color tint = WHITE);
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawRectLines(int x, int y, int w, int h, Color color);
	void DrawLine(float x1, float y1, float x2, float y2, float thickness, Color color);
	void DrawText(const char* text, int x, int y, int fontSize, Color color);
	void DrawCenteredText(const char* text, int fontSize, int posY, Color color);

private:
	std::unique_ptr<Color> clearColor;
};

#endif