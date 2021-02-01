#pragma once

#include "GameState.h"
#include "TGUI/TGUI.hpp"

class FinalScreen : public GameState
{
public:
	using GameState::GameState;

	void init() override;

	void update(float delta) override;
	void draw() override;
	void setupGUI();
	void exit() override;

private:
	View mView;

	bool mIsInit = false;

};
