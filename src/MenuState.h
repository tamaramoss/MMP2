#pragma once

#include "GameState.h"

#include <SFML/Graphics/Text.hpp>

class MenuState : public GameState
{
public:
	using GameState::GameState;

	void init() override;

	void update(float delta) override;
	void draw() override;

private:
	// should be gameobject too
	Text mText;
	Font mFont;
	View mView;

	bool mIsInit = false;
};
