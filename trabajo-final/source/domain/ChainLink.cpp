#include "ChainLink.h"
#include "../core/PhysicsConstants.h"
#include <cmath>

ChainLink::ChainLink(b2World& world, GameContext& context, float startX, float startY, float angle, float density, float friction, float restitution, float linearDamping, float angularDamping)
	: context(context), density(density), friction(friction), restitution(restitution), linearDamping(linearDamping), angularDamping(angularDamping)
{
	hitbox = RectangleEntity::CreateDynamic(world, startX - LINK_WIDTH / 2.0f, startY - LINK_HEIGHT / 2.0f, LINK_WIDTH, LINK_HEIGHT, angle * RAD2DEG, Fade(BLACK, 0.2f), density, friction, restitution);
	hitbox->GetBody()->SetLinearDamping(linearDamping);
	hitbox->GetBody()->SetAngularDamping(angularDamping);

	b2Filter filter;
	filter.groupIndex = PLAYER_GROUP_INDEX;
	hitbox->GetBody()->GetFixtureList()->SetFilterData(filter);

	bodyData = { BodyTag::ChainLink, this };
	hitbox->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(&bodyData);
}

ChainLink::~ChainLink()
{
}

void ChainLink::Launch(float angleRad, float speed)
{
	hitbox->GetBody()->SetLinearVelocity(b2Vec2(speed * cosf(angleRad), speed * sinf(angleRad)));
}

void ChainLink::Update(float deltaTime)
{
	hitbox->Update(deltaTime);
}

void ChainLink::Render(Renderer& renderer)
{
	if (texture.id > 0) {
		renderer.DrawSprite(texture, { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
			{ hitbox->position.x, hitbox->position.y, LINK_WIDTH, LINK_HEIGHT }, hitbox->angle);
	}
	else {
		hitbox->Render(renderer);
	}

	if (context.debugMode) {
		hitbox->Render(renderer);
	}
}
