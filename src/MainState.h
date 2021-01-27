#pragma once

#include "GameState.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"
#include "PhysicsManager.h"

class FINALFRONTIER_API MainState : public GameState
{
public:
	MainState(GameStateManager* gameStateManager, Game* game);

	void init() override;
	void exit() override;

	void update(float deltaTime) override;
	void draw() override;

private:
	SpriteManager mSpriteManager;
	GameObjectManager mGameObjectManager;
	PhysicsManager mPhysicsManager;
};
