/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

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
	PhysicsManager mPhysicsManager;

	float mNextStateTimer = 5.0f;
	float mTimer = 0.0f;

	bool mIsInit = false;
};
