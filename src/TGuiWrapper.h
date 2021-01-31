#pragma once
#include <TGUI/TGUI.hpp>
#include "Game.h"

class Game;

class TGuiWrapper
{
public:
	static TGuiWrapper& getInstance();

	void process(sf::Event event);
	tgui::Gui& getGui() { return mGui; }

	bool isGuiActive() { return mGuiActive; }
	void setGuiStatus(bool isActive) { mGuiActive = isActive; }

	void addButton(tgui::Button::Ptr button, bool focused = false);
	
	void setGame(Game* game);
	
private:
	struct Button
	{
		bool isFocused;
		tgui::Button::Ptr button;
	};
	bool mGuiActive = false;
	tgui::Gui mGui;
	Game* mGame = nullptr;

	std::vector<Button> mButtons;
	int mFocusedButtonIndex = 0;
};








