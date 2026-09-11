#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	buffer.head = 0;
	buffer.tail = 0;
	buffer.buffer = (Particle*)RL_CALLOC(MAX_PARTICLES, sizeof(Particle));
}

ParticleSystem::~ParticleSystem()
{
	RL_FREE(buffer.buffer);
}

void ParticleSystem::Emit(Vector2 position, ParticleType type, int count)
{
	for (int i = 0; i < count; i++) {
		EmitParticle(&buffer, position, type);
	}
}

void ParticleSystem::Update(float deltaTime, int screenWidth, int screenHeight)
{
	UpdateParticles(&buffer, screenWidth, screenHeight);
	UpdateCircularBuffer(&buffer);
}

void ParticleSystem::Draw()
{
	DrawParticles(&buffer);
}
