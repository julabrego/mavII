#include "BuildingBlock.h"
#include "RectangleEntity.h"
#include "../core/PhysicsConstants.h"
#include "../core/GameplayConstants.h"
#include <cmath>

std::unique_ptr<BuildingBlock> BuildingBlock::Create(b2World& world, int row, int column,
	float x, float y, float width, float height, Color color,
	float density, float friction, float restitution, float borderThickness)
{
	auto shape = RectangleEntity::CreateDynamic(world, x, y, width, height, 0.0f, color,
		density, friction, restitution, borderThickness);
	Vector2 homeCenter = { x + width / 2.0f, y + height / 2.0f };
	return std::unique_ptr<BuildingBlock>(new BuildingBlock(std::move(shape), row, column,
		homeCenter, width * BLOCK_FALL_TOLERANCE_RATIO, color));
}

BuildingBlock::BuildingBlock(std::unique_ptr<RectangleEntity> shape, int row, int column,
	Vector2 homeCenter, float fallTolerance, Color baseColor)
	: shape(std::move(shape))
	, row(row)
	, column(column)
	, homeCenter(homeCenter)
	, fallTolerance(fallTolerance)
	, baseColor(baseColor)
{
}

void BuildingBlock::Update(float deltaTime)
{
	shape->Update(deltaTime);

	if (!demolished) {
		Vector2 center = shape->GetCenter();
		if (fabsf(center.x - homeCenter.x) > fallTolerance
			|| fabsf(center.y - homeCenter.y) > fallTolerance) {
			demolished = true;
		}
		return;
	}

	if (landed) {
		return;
	}

	b2Body* body = shape->GetBody();
	b2Vec2 velocity = body->GetLinearVelocity();
	bool atRest = velocity.LengthSquared() <= SETTLE_SPEED_THRESHOLD * SETTLE_SPEED_THRESHOLD
		&& fabsf(body->GetAngularVelocity()) <= SETTLE_ANGULAR_THRESHOLD;

	if (atRest) {
		landedTimer += deltaTime;
		if (landedTimer >= BLOCK_LAND_CONFIRM_TIME) {
			landed = true;
		}
	}
	else {
		landedTimer = 0.0f;
	}
}

static Color DarkenColor(Color color)
{
	return Color{
		static_cast<unsigned char>(color.r * BLOCK_RUBBLE_DARKNESS),
		static_cast<unsigned char>(color.g * BLOCK_RUBBLE_DARKNESS),
		static_cast<unsigned char>(color.b * BLOCK_RUBBLE_DARKNESS),
		color.a };
}

void BuildingBlock::Render(Renderer& renderer)
{
	shape->color = shadowed ? DarkenColor(baseColor) : baseColor;
	shape->Render(renderer);
}

b2Body* BuildingBlock::GetBody() const
{
	return shape->GetBody();
}

bool BuildingBlock::IsStanding() const
{
	return !demolished;
}
