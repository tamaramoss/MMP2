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
	void exit() override;

private:
	// should be gameobject too
	Text mText;
	Font mFont;
	View mView;

	tgui::Theme mTheme;

	bool mIsInit = false;

};
