#pragma once

#include "GameObjectManager.h"
#include "GameState.h"
#include "SpriteManager.h"
#include "TGuiWrapper.h"


class ControlState : public GameState
{
public:
	//using GameState::GameState;

	ControlState(GameStateManager* gameStateManager, Game* game);

	void init() override;

	void update(float delta) override;
	void draw() override;

	void exit() override;

private:
	SpriteManager mSpriteManager;
	GameObjectManager mGameObjectManager;
	sf::Music mMusic;
	bool mIsInit = false;

};