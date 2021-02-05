#pragma once

#include "GameObjectManager.h"
#include "GameState.h"
#include "SpriteManager.h"
#include "TGuiWrapper.h"


class FinalScreen : public GameState
{
public:
	FinalScreen(GameStateManager* gameStateManager, Game* game);

	void init() override;

	void update(float delta) override;
	void draw() override;

	void exit() override;

private:
	SpriteManager mSpriteManager;
	GameObjectManager mGameObjectManager;
	std::shared_ptr<TGuiWrapper> mGuiManager;
	sf::Music mMusic;
	
	bool mIsInit = false;
};
