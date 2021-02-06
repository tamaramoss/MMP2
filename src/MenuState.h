#pragma once

#include "GameObjectManager.h"
#include "GameState.h"
#include "SpriteManager.h"
#include "GuiManager.h"


class MenuState : public GameState
{
public:
	//using GameState::GameState;

	MenuState(GameStateManager* gameStateManager, Game* game);

	void init() override;

	void update(float delta) override;
	void draw() override;

	void exit() override;

private:
	View mView;
	SpriteManager mSpriteManager;
	std::shared_ptr<GuiManager> mGuiManager;
	sf::Music mMusic;
	bool mIsInit = false;

};
