#pragma once

#include "GameState.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"
#include "PhysicsManager.h"

class FINALFRONTIER_API TestState : public GameState
{
public:
	TestState(GameStateManager* gameStateManager, Game* game);

	void init() override;
	void exit() override;

	void update(float deltaTime) override;
	void draw() override;

private:
	SpriteManager mSpriteManager;
	GameObjectManager mGameObjectManager;
	PhysicsManager mPhysicsManager;
};
