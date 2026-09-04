#include "Chain.h"
#include "WreckingBall.h"
#include "ChainLink.h"
#include "../core/PhysicsConstants.h"
#include "../core/GameplayConstants.h"
#include "../core/Renderer.h"
#include <cmath>

Chain::Chain(GameContext& context)
	: context(context)
{
}

Chain::~Chain() = default;

void Chain::Reset()
{
	ball.reset();
	links.clear();
	joints.clear();
	tether = nullptr;
}

void Chain::SpawnLink(b2Body* playerBody, float angleRad, float spawnX, float spawnY)
{
	if (!world) return;

	if (!ball) {
		auto b = std::make_unique<WreckingBall>(*world, context, spawnX, spawnY, PROJECTILE_RADIUS);
		b->Launch(angleRad, SHOOT_SPEED);
		ball = std::move(b);
	}

	auto link = std::make_unique<ChainLink>(*world, context, spawnX, spawnY, angleRad);
	link->Launch(angleRad, SHOOT_SPEED);
	links.push_back(std::move(link));

	float linkGapInMeters = CHAIN_LINK_SPACING * METERS_PER_PIXEL;
	b2DistanceJointDef jointDef;

	jointDef.bodyA = links.size() == 1 ? ball->GetBody() : links[links.size() - 2]->GetBody();
	jointDef.bodyB = links.back()->GetBody();

	float borderA = links.size() == 1 ? 0.0f : -(LINK_WIDTH / 2.0f) * METERS_PER_PIXEL;
	float borderB = (LINK_WIDTH / 2.0f) * METERS_PER_PIXEL;

	jointDef.localAnchorA.Set(borderA, 0.0f);
	jointDef.localAnchorB.Set(borderB, 0.0f);
	jointDef.length = linkGapInMeters;
	jointDef.minLength = 0.0f;
	jointDef.maxLength = linkGapInMeters;
	joints.push_back(static_cast<b2DistanceJoint*>(world->CreateJoint(&jointDef)));

	DestroyTether();

	b2DistanceJointDef tetherDef;
	tetherDef.bodyA = playerBody;
	tetherDef.bodyB = links.back()->GetBody();
	tetherDef.localAnchorA.Set(LAUNCH_OFFSET * METERS_PER_PIXEL, 0.0f);
	tetherDef.localAnchorB.Set(-(LINK_WIDTH / 2.0f) * METERS_PER_PIXEL, 0.0f);
	tetherDef.length = CHAIN_TETHER_MAX_LENGTH * METERS_PER_PIXEL;
	tetherDef.minLength = 0.0f;
	tetherDef.maxLength = CHAIN_TETHER_MAX_LENGTH * METERS_PER_PIXEL;
	tether = static_cast<b2DistanceJoint*>(world->CreateJoint(&tetherDef));

	if (NeedsCollisions()) {
		EnableCollisions();
	}
}

void Chain::Pull(float deltaTime, b2Body* playerBody)
{
	if (!ball || !tether) return;

	b2Body* target = links.empty() ? ball->GetBody() : links.back()->GetBody();

	b2Vec2 cannonAnchor = playerBody->GetWorldPoint(
		b2Vec2(LAUNCH_OFFSET * METERS_PER_PIXEL, 0.0f));
	b2Vec2 linkPos = target->GetPosition();
	b2Vec2 dir = cannonAnchor - linkPos;
	float dist = dir.Length();

	if (dist < LINK_WIDTH * METERS_PER_PIXEL) {
		ConsumeLink(playerBody);
		return;
	}

	if (dist > 0.001f) {
		dir.Normalize();
		float mass = target->GetMass();
		float force = mass * PULL_SPEED * METERS_PER_PIXEL * PULL_FORCE_MULTIPLIER;
		target->ApplyForceToCenter(b2Vec2(dir.x * force, dir.y * force), true);
	}
}

void Chain::ConsumeLink(b2Body* playerBody)
{
	if (!ball || !world) return;

	if (links.empty()) {
		world->DestroyBody(ball->GetBody());
		ball.reset();
		return;
	}

	DestroyTether();
	world->DestroyBody(links.back()->GetBody());
	links.pop_back();
	if (!joints.empty()) {
		joints.pop_back();
	}

	if (links.empty()) {
		world->DestroyBody(ball->GetBody());
		ball.reset();
		return;
	}

	ReconnectTether(playerBody);
}

void Chain::CleanupOffScreen(float screenWidth, float screenHeight, b2Body* playerBody)
{
	if (!world) return;

	for (auto it = links.begin(); it != links.end(); ) {
		float x = (*it)->GetBody()->GetPosition().x * PIXELS_PER_METER;
		float y = (*it)->GetBody()->GetPosition().y * PIXELS_PER_METER;

		if (x < -100.0f || x > screenWidth + 100.0f || y < -100.0f || y > screenHeight + 100.0f) {
			int idx = static_cast<int>(it - links.begin());

			if (idx < static_cast<int>(joints.size())) {
				joints.erase(joints.begin() + idx);
			}

			DestroyTether();
			world->DestroyBody((*it)->GetBody());
			it = links.erase(it);

			if (!links.empty()) {
				ReconnectTether(playerBody);
			}
		}
		else {
			++it;
		}
	}
}

void Chain::EnableCollisions()
{
	if (!world) return;

	b2Filter filter;
	filter.groupIndex = 0;

	if (ball) {
		ball->GetBody()->GetFixtureList()->SetFilterData(filter);
	}
	for (auto& link : links) {
		link->GetBody()->GetFixtureList()->SetFilterData(filter);
	}
}

void Chain::Update(float deltaTime)
{
	if (ball) {
		ball->Update(deltaTime);
	}
	for (auto& link : links) {
		link->Update(deltaTime);
	}
}

void Chain::Render(Renderer& renderer)
{
	if (ball) {
		ball->Render(renderer);
	}
	for (auto& link : links) {
		link->Render(renderer);
	}
}

void Chain::RenderDebug(Renderer& renderer)
{
	for (auto& joint : joints) {
		b2Vec2 anchorA = joint->GetAnchorA();
		b2Vec2 anchorB = joint->GetAnchorB();
		DrawCircle(static_cast<int>(anchorA.x * PIXELS_PER_METER), static_cast<int>(anchorA.y * PIXELS_PER_METER), 3.0f, RED);
		DrawCircle(static_cast<int>(anchorB.x * PIXELS_PER_METER), static_cast<int>(anchorB.y * PIXELS_PER_METER), 3.0f, RED);
	}
	if (tether) {
		b2Vec2 anchorA = tether->GetAnchorA();
		b2Vec2 anchorB = tether->GetAnchorB();
		DrawCircle(static_cast<int>(anchorA.x * PIXELS_PER_METER), static_cast<int>(anchorA.y * PIXELS_PER_METER), 3.0f, RED);
		DrawCircle(static_cast<int>(anchorB.x * PIXELS_PER_METER), static_cast<int>(anchorB.y * PIXELS_PER_METER), 3.0f, RED);
	}
}

void Chain::DestroyTether()
{
	if (tether && world) {
		world->DestroyJoint(tether);
		tether = nullptr;
	}
}

void Chain::ReconnectTether(b2Body* playerBody)
{
	if (links.empty() || !world) return;

	b2DistanceJointDef tetherDef;
	tetherDef.bodyA = playerBody;
	tetherDef.bodyB = links.back()->GetBody();
	tetherDef.localAnchorA.Set(LAUNCH_OFFSET * METERS_PER_PIXEL, 0.0f);
	tetherDef.localAnchorB.Set(-(LINK_WIDTH / 2.0f) * METERS_PER_PIXEL, 0.0f);

	b2Vec2 anchorA = playerBody->GetWorldPoint(tetherDef.localAnchorA);
	b2Vec2 anchorB = links.back()->GetBody()->GetWorldPoint(tetherDef.localAnchorB);
	float actualLength = b2Distance(anchorA, anchorB);

	tetherDef.length = actualLength;
	tetherDef.minLength = 0.0f;
	tetherDef.maxLength = actualLength;
	tether = static_cast<b2DistanceJoint*>(world->CreateJoint(&tetherDef));
}
