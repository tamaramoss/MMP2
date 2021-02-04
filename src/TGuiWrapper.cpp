#include "stdafx.h"

#include "TGuiWrapper.h"


#include "AnimationComponent.h"
#include "ColliderComponent.h"
#include "InputManager.h"


TGuiWrapper::TGuiWrapper(Game* game) : mGame(game)
{
	
}

void TGuiWrapper::process()
{
	if (mButtons.size() == 0)
		return;

	if (InputManager::getInstance().isButtonPressed("Selected") && mFocusedButtonIndex != -1)
	{
		if (mButtons[mFocusedButtonIndex]->getId() == "StartButton")
		{
			mGame->getGameStateManager().setState("MainState");
		}
	}

	float position = InputManager::getInstance().getAxisPosition("DPad", 0);

 	if (position != 0)
	{
		if (position < 0.f)
		{
			updateFocusedButtonIndex(1);
		}
		if (position > 0.f)
		{
			updateFocusedButtonIndex(-1);
		}
	}
}


void TGuiWrapper::addButton(GameObject::ptr button, bool focused)
{
	if (focused)
		mFocusedButtonIndex = mButtons.size();

	mButtons.push_back(button);
}

void TGuiWrapper::updateFocusedButtonIndex(int step)
{
	mButtons[mFocusedButtonIndex]->get_component<AnimationComponent>()->setAnimation("Default");
	
	mFocusedButtonIndex += step;

	if (mFocusedButtonIndex < 0) mFocusedButtonIndex = mButtons.size() - 1;
	if (mFocusedButtonIndex >= mButtons.size()) mFocusedButtonIndex = 0;

	mButtons[mFocusedButtonIndex]->get_component<AnimationComponent>()->setAnimation("Focused");

}

void TGuiWrapper::exit()
{
	mButtons.clear();
	mFocusedButtonIndex = -1;
}
