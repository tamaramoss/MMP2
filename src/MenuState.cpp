#include "stdafx.h"

#include "MenuState.h"

#include "Game.h"
#include "GameStateManager.h"

#include "InputManager.h"
#include <iostream>

using namespace std;

void
MenuState::init()
{
	if (mIsInit)
		return;

	if (!mFont.loadFromFile("../assets/arial.ttf"))
	{
		err() << "Could not load font\n";
		return;
	}
	mText.setPosition(mGame->getWindow().getView().getCenter());
	mText.setString("Press space to continue");
	mText.setFillColor(Color::White);
	mText.setFont(mFont);
	mText.setOrigin(
		mText.getLocalBounds().width * 0.5f,
		mText.getLocalBounds().height * 0.5f);

	mView = mGame->getWindow().getView();

	mIsInit = true;
}

void MenuState::update(float delta)
{
	mGame->getWindow().setView(mView);

	if (InputManager::getInstance().isButtonPressed("Select"))
	{
		mGameStateManager->setState("MainState");
		std::cout << "select pressed" << std::endl;
	}
}

void MenuState::draw()
{
	mGame->getWindow().draw(mText);
}
