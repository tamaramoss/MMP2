#pragma once

#include "GameObjectManager.h"
#include "GameState.h"
#include "SpriteManager.h"
#include "GuiManager.h"


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

	std::shared_ptr<GuiManager> mGuiManager;

	
	bool mIsInit = false;
};
