#pragma once

#include "GameState.h"
#include "TGUI/TGUI.hpp"
#include <SFML/Graphics/Text.hpp>

class MenuState : public GameState
{
public:
	using GameState::GameState;

	void init() override;

	void update(float delta) override;
	void draw() override;
	void setupGUI();

private:
	// should be gameobject too
	Text mText;
	Font mFont;
	View mView;

	tgui::Gui mGUI;

	bool mIsInit = false;

};
