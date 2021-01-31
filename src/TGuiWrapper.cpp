#include "stdafx.h"

#include "TGuiWrapper.h"
#include <utility>

TGuiWrapper& TGuiWrapper::getInstance()
{
	static TGuiWrapper instance;
	return instance;
}

void TGuiWrapper::process(sf::Event event)
{
	if (!mGuiActive)
		return;

	if (event.type == sf::Event::JoystickButtonPressed)
	{
		mButtons[mFocusedButtonIndex].button->onPress.emit(mButtons[mFocusedButtonIndex].button.get(), mButtons[mFocusedButtonIndex].button->getText());
	}

	if (event.type == sf::Event::JoystickMoved)
	{
		auto axis = event.joystickMove.axis;

		if (!axis == sf::Joystick::PovY || !axis == sf::Joystick::Y) return;

		float position = event.joystickMove.position;

		if (position < 15.f && position > -15.f) position = 0.0f;

		if (position < 0 && mGui.focusNextWidget())
		{
			mButtons[mFocusedButtonIndex].isFocused = false;
			mFocusedButtonIndex++;
			mButtons[mFocusedButtonIndex].isFocused = true;

		}
		if (position > 0 && mGui.focusPreviousWidget())
		{
			mButtons[mFocusedButtonIndex].isFocused = false;
			mFocusedButtonIndex--;
			mButtons[mFocusedButtonIndex].isFocused = true;
		}
	}
}

void TGuiWrapper::setGame(Game* game)
{
	mGame = game;

	mGui.setTarget(mGame->getWindow());
}

void TGuiWrapper::addButton(tgui::Button::Ptr button, bool focused)
{
	auto buttonStruct = Button{ focused, button };

	if (focused)
		mFocusedButtonIndex = mButtons.size();

	mButtons.push_back(buttonStruct);

	mGui.add(button);
}
