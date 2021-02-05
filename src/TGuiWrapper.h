#pragma once

#include "Game.h"
#include "GameObject.h"

class Game;

class TGuiWrapper
{
public:
	TGuiWrapper(Game* game);

	void process(float deltaTime);
	
	void exit();

	void addButton(GameObject::ptr button, bool focused = false);

private:

	void updateFocusedButtonIndex(int step);
	Game* mGame;
	std::vector<GameObject::ptr> mButtons;
	int mFocusedButtonIndex = -1;

	bool mMovedLastFrame = false;
	float mTimer = 0.f;
	float mNextButtonTimer = 0.2f;
};










