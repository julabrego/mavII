#include "WreckingBall.h"
#include "../core/PhysicsConstants.h"
#include <cmath>

WreckingBall::WreckingBall(b2World& world, float startX, float startY, float radius, float density, float friction, float restitution, float linearDamping, float angularDamping)
	: radius(radius), density(density), friction(friction), restitution(restitution), linearDamping(linearDamping), angularDamping(angularDamping)
{
	hitbox = CircleEntity::CreateDynamic(world, startX, startY, radius, Fade(YELLOW, 0.5f), density, friction, restitution);
	hitbox->GetBody()->SetLinearDamping(linearDamping);
	hitbox->GetBody()->SetAngularDamping(angularDamping);

	b2Filter filter;
	filter.groupIndex = PLAYER_GROUP_INDEX;
	hitbox->GetBody()->GetFixtureList()->SetFilterData(filter);

	bodyData = { BodyTag::WreckingBall, this };
	hitbox->GetBody()->GetUserData().pointer = reinterpret_cast<uintptr_t>(&bodyData);
}

WreckingBall::~WreckingBall()
{
}

void WreckingBall::Launch(float angleRad, float speed)
{
	hitbox->GetBody()->SetLinearVelocity(b2Vec2(speed * cosf(angleRad), speed * sinf(angleRad)));
}

void WreckingBall::Update(float deltaTime)
{
	hitbox->Update(deltaTime);
}

void WreckingBall::Render(Renderer& renderer)
{
	hitbox->Render(renderer);
}
