#pragma once

#include "GameObjectManager.h"
#include "GameState.h"
#include "SpriteManager.h"
#include "TGuiWrapper.h"
#include "TGUI/TGUI.hpp"

class MenuState : public GameState
{
public:
	//using GameState::GameState;

	MenuState(GameStateManager* gameStateManager, Game* game);

	void init() override;

	void update(float delta) override;
	void draw() override;
	void setupGUI();
	void exit() override;

private:
	View mView;
	SpriteManager mSpriteManager;
	GameObjectManager mGameObjectManager;
	std::shared_ptr<TGuiWrapper> mGuiManager;

	bool mIsInit = false;

};
