#include "Particle.h"

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

static Particle* AddToCircularBuffer(CircularBuffer* circularBuffer)
{
    Particle* particle = NULL;

    // Check if buffer full
    if (((circularBuffer->head + 1) % MAX_PARTICLES) != circularBuffer->tail)
    {
        // Add new particle to the head position and advance head
        particle = &circularBuffer->buffer[circularBuffer->head];
        circularBuffer->head = (circularBuffer->head + 1) % MAX_PARTICLES;
    }

    return particle;
}

void EmitParticle(CircularBuffer* circularBuffer, Vector2 emitterPosition, ParticleType type)
{
    Particle* newParticle = AddToCircularBuffer(circularBuffer);

    // If buffer is full, newParticle is NULL
    if (newParticle != NULL)
    {
        // Fill particle properties
        newParticle->position = emitterPosition;
        newParticle->alive = true;
        newParticle->lifeTime = 0.0f;
        newParticle->type = type;
        float speed = (float)(rand() % 10) / 5.0f;
        switch (type)
        {
        case SMOKE:
        {
            newParticle->radius = 7.0f;
            newParticle->color = GRAY;
        } break;
        case SPARK:
        {
            newParticle->radius = 2.5f;
            newParticle->color = ORANGE;
        } break;
        default: break;
        }

        float direction = (float)(rand() % 360);
        newParticle->velocity = { speed * cosf(direction * DEG2RAD), speed * sinf(direction * DEG2RAD) };
    }
}

void UpdateParticles(CircularBuffer* circularBuffer, int screenWidth, int screenHeight)
{
    for (int i = circularBuffer->tail; i != circularBuffer->head; i = (i + 1) % MAX_PARTICLES)
    {
        // Update particle life and positions
        circularBuffer->buffer[i].lifeTime += 1.0f / 60.0f; // 60 FPS -> 1/60 seconds per frame

        switch (circularBuffer->buffer[i].type)
        {
        case SMOKE:
        {
            circularBuffer->buffer[i].position.x += circularBuffer->buffer[i].velocity.x;
            circularBuffer->buffer[i].velocity.y -= 0.05f;  // Upwards
            circularBuffer->buffer[i].position.y += circularBuffer->buffer[i].velocity.y;
            circularBuffer->buffer[i].radius += 0.5f;       // Increment radius: smoke expands
            circularBuffer->buffer[i].color.a -= 4;         // Decrement alpha: smoke fades

            // If alpha transparent, particle dies
            if (circularBuffer->buffer[i].color.a < 4) circularBuffer->buffer[i].alive = false;
        } break;
        case SPARK:
        {
            circularBuffer->buffer[i].position.x += circularBuffer->buffer[i].velocity.x;
            circularBuffer->buffer[i].velocity.y += 0.2f;   // Gravity
            circularBuffer->buffer[i].position.y += circularBuffer->buffer[i].velocity.y;
        } break;
        default: break;
        }

        // Disable particle when out of screen
        Vector2 center = circularBuffer->buffer[i].position;
        float radius = circularBuffer->buffer[i].radius;

        if ((center.x < -radius) || (center.x > (screenWidth + radius)) ||
            (center.y < -radius) || (center.y > (screenHeight + radius)))
        {
            circularBuffer->buffer[i].alive = false;
        }
    }
}

void UpdateCircularBuffer(CircularBuffer* circularBuffer)
{
    // Update circular buffer: advance tail over dead particles
    while ((circularBuffer->tail != circularBuffer->head) && !circularBuffer->buffer[circularBuffer->tail].alive)
    {
        circularBuffer->tail = (circularBuffer->tail + 1) % MAX_PARTICLES;
    }
}

void DrawParticles(CircularBuffer* circularBuffer)
{
    for (int i = circularBuffer->tail; i != circularBuffer->head; i = (i + 1) % MAX_PARTICLES)
    {
        if (circularBuffer->buffer[i].alive)
        {
            DrawCircleV(circularBuffer->buffer[i].position,
                circularBuffer->buffer[i].radius,
                circularBuffer->buffer[i].color);
        }
    }
}