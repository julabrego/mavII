#pragma once

#include <cstdint>

constexpr float SCALE = 30.0f;
constexpr float PIXELS_PER_METER = SCALE;
constexpr float METERS_PER_PIXEL = 1.0f / SCALE;

constexpr int16_t PLAYER_GROUP_INDEX = -1;

constexpr float GRAVITY = 9.8f;

constexpr float SETTLE_SPEED_THRESHOLD = 0.15f;
constexpr float SETTLE_ANGULAR_THRESHOLD = 0.15f;
constexpr float MIN_EMIT_SPEED = 1.0f;
