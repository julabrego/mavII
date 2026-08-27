#pragma once

#include <box2d.h>
#include <memory>
#include <vector>
#include "../core/GameplayConstants.h"
#include "ChainLink.h"
#include "WreckingBall.h"

class Chain {
public:
	Chain() = default;
	Chain(b2World& world);
	~Chain();

	void SetWorld(b2World& world) { this->world = &world; }

	void SpawnLink(b2Body* playerBody, float angleRad, float spawnX, float spawnY);
	void Pull(float deltaTime, b2Body* playerBody);
	void CleanupOffScreen(float screenWidth, float screenHeight, b2Body* playerBody);
	void Reset();
	void EnableCollisions();

	void Update(float deltaTime);
	void Render(Renderer& renderer);
	void RenderDebug(Renderer& renderer);

	bool HasBall() const { return ball != nullptr; }
	bool IsEmpty() const { return !ball && links.empty(); }
	int LinkCount() const { return static_cast<int>(links.size()); }
	bool AtCapacity() const { return static_cast<int>(links.size()) >= MAX_CHAIN_LINKS; }
	bool NeedsCollisions() const { return AtCapacity(); }
	WreckingBall* GetBall() const { return ball.get(); }
	b2Body* GetLastLinkBody() const { return links.empty() ? nullptr : links.back()->GetBody(); }

private:
	void ConsumeLink(b2Body* playerBody);
	void DestroyTether();
	void ReconnectTether(b2Body* playerBody);

	b2World* world = nullptr;
	std::unique_ptr<WreckingBall> ball;
	std::vector<std::unique_ptr<ChainLink>> links;
	std::vector<b2DistanceJoint*> joints;
	b2DistanceJoint* tether = nullptr;
};
