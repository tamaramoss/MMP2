#include "stdafx.h"

#include "TGuiWrapper.h"

#include <iostream>
#include <utility>

TGuiWrapper& TGuiWrapper::getInstance()
{
	static TGuiWrapper instance;
	return instance;
}

void TGuiWrapper::process(sf::Event event)
{
	if (mButtons.size() == 0)
		return;

	if (event.type == sf::Event::JoystickButtonPressed && sf::Joystick::isButtonPressed(0, 1) && mFocusedButtonIndex != -1)
	{
		mButtons[mFocusedButtonIndex]->onPress.emit(mButtons[mFocusedButtonIndex].get(), mButtons[mFocusedButtonIndex]->getText());
	}

	if (event.type == sf::Event::JoystickMoved)
	{
		auto axis = event.joystickMove.axis;

		if (!(axis == sf::Joystick::PovY) && !(axis == sf::Joystick::Y)) return;

		float position = event.joystickMove.position;

		if (position < 15.f && position > -15.f) position = 0.0f;

		if (position < 0.f && mGui.focusNextWidget())
		{
			updateFocusedButtonIndex(1);
			//std::cout << mFocusedButtonIndex << std::endl;
		}
		if (position > 0.f && mGui.focusPreviousWidget())
		{
			updateFocusedButtonIndex(-1);
			//std::cout << mFocusedButtonIndex << std::endl;
		}
	}
}

void TGuiWrapper::exit()
{
	mButtons.clear();
	mGui.removeAllWidgets();
	mFocusedButtonIndex = -1;
}

void TGuiWrapper::setGame(Game* game)
{
	mGame = game;

	mGui.setTarget(mGame->getWindow());
}

void TGuiWrapper::addButton(tgui::Button::Ptr button, bool focused)
{
	if (focused)
		mFocusedButtonIndex = mButtons.size();

	mButtons.push_back(button);

	mGui.add(button);
}

void TGuiWrapper::updateFocusedButtonIndex(int step)
{
	mFocusedButtonIndex += step;

	if (mFocusedButtonIndex < 0) mFocusedButtonIndex = mButtons.size() - 1;
	if (mFocusedButtonIndex >= mButtons.size()) mFocusedButtonIndex = 0;
}
