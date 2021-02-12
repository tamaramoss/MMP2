/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

#include "Game.h"
#include "GameObject.h"

class Game;

class GuiManager
{
public:
	GuiManager(Game* game);

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










