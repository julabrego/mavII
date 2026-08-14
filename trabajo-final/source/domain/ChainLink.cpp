#include "ChainLink.h"
#include "../core/PhysicsConstants.h"
#include <cmath>

ChainLink::ChainLink(b2World& world, float startX, float startY, float radius, float density, float friction, float restitution, float linearDamping, float angularDamping)
	: radius(radius), density(density), friction(friction), restitution(restitution), linearDamping(linearDamping), angularDamping(angularDamping)
{
	hitbox = CircleEntity::CreateDynamic(world, startX, startY, radius, Fade(BLACK, 0.2f), density, friction, restitution);
	hitbox->GetBody()->SetLinearDamping(linearDamping);
	hitbox->GetBody()->SetAngularDamping(angularDamping);
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
	hitbox->Render(renderer);
}
