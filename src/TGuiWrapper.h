#pragma once
#include <TGUI/TGUI.hpp>
#include "Game.h"
#include "GameObject.h"

class Game;

class TGuiWrapper
{
public:
	TGuiWrapper(Game* game);
	
	void setGame(Game* game);

	void process();

	void draw();
	
	void exit();

	void addButton(GameObject::ptr button, bool focused = false);

private:

	void updateFocusedButtonIndex(int step);
	Game* mGame;
	std::vector<GameObject::ptr> mButtons;
	int mFocusedButtonIndex = -1;
};










