#pragma once

#include "raylib.h"

#include <stdlib.h>         // Required for: calloc(), free()
#include <math.h>           // Required for: cosf(), sinf()

#define MAX_PARTICLES 3000  // Max number of particles

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum ParticleType {
    SMOKE = 0,
    SPARK
} ParticleType;

static const char particleTypeNames[2][10] = { "SMOKE", "SPARK" };

typedef struct Particle {
    ParticleType type;      // Particle type (SMOKE, SPARK)
    Vector2 position;       // Particle position on screen
    Vector2 velocity;       // Particle current speed and direction
    float radius;           // Particle radius
    Color color;            // Particle color

    float lifeTime;         // Particle life time
    bool alive;             // Particle alive: inside screen and life time
} Particle;

typedef struct CircularBuffer {
    int head;               // Index for the next write
    int tail;               // Index for the next read
    Particle* buffer;       // Particle buffer array
} CircularBuffer;

void EmitParticle(CircularBuffer* circularBuffer, Vector2 emitterPosition, ParticleType type);
void UpdateParticles(CircularBuffer* circularBuffer, int screenWidth, int screenHeight);
void UpdateCircularBuffer(CircularBuffer* circularBuffer);
void DrawParticles(CircularBuffer* circularBuffer);
