#pragma once

#include "raylib.h"
#include "Particle.h"

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();

	void Emit(Vector2 position, ParticleType type, int count = 1);
	void Update(float deltaTime, int screenWidth, int screenHeight);
	void Draw();

private:
	CircularBuffer buffer;
};
