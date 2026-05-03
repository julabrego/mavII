#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include <memory>

class Renderer
{
public:
	Renderer(Color clearColor);
	~Renderer();

	void Begin();
	void End();

private:
	std::unique_ptr<Color> clearColor;
};

#endif