#pragma once
#include <TGUI/TGUI.hpp>
#include "Game.h"

class Game;

class TGuiWrapper
{
public:
	static TGuiWrapper& getInstance();
	void setGame(Game* game);


	void process(sf::Event event);
	void exit();

	
	tgui::Gui& getGui() { return mGui; }

	void addButton(tgui::Button::Ptr button, bool focused = false);

private:
	void updateFocusedButtonIndex(int step);

	tgui::Gui mGui;
	Game* mGame = nullptr;

	std::vector<tgui::Button::Ptr> mButtons;
	int mFocusedButtonIndex = -1;
};










