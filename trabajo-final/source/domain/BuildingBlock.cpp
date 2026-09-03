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
	if (texture && texture->id > 0) {
		float tileWidth = texture->width / static_cast<float>(textureColumns);
		float tileHeight = texture->height / static_cast<float>(textureRows);
		Rectangle src = { tileColumn * tileWidth, tileRow * tileHeight, tileWidth, tileHeight };
		Rectangle dst = { shape->position.x + shape->width / 2.0f, shape->position.y + shape->height / 2.0f, shape->width, shape->height };
		Vector2 origin = { shape->width / 2.0f, shape->height / 2.0f };
		float angle = shape->GetBody()->GetAngle() * RAD2DEG;
		DrawTexturePro(*texture, src, dst, origin, angle, WHITE);
	}
	else {
		shape->color = shadowed ? DarkenColor(baseColor) : baseColor;
		shape->Render(renderer);
	}
}

b2Body* BuildingBlock::GetBody() const
{
	return shape->GetBody();
}

bool BuildingBlock::IsStanding() const
{
	return !demolished;
}

void BuildingBlock::MarkDemolished()
{
	if (!demolished) {
		demolished = true;
		landedTimer = 0.0f;
	}
}

void BuildingBlock::SetTexture(Texture2D* tex, int cols, int rows, int tCol, int tRow)
{
	texture = tex;
	textureColumns = cols;
	textureRows = rows;
	tileColumn = tCol;
	tileRow = tRow;
}
