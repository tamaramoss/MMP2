#include "stdafx.h"

#include "TGuiWrapper.h"


#include "AnimationComponent.h"
#include "ColliderComponent.h"
#include "InputManager.h"
#include "SoundComponent.h"


TGuiWrapper::TGuiWrapper(Game* game) : mGame(game)
{
	
}

void TGuiWrapper::process(float deltaTime)
{
	if (mButtons.size() == 0)
		return;

	if (InputManager::getInstance().isButtonPressed("Select") && mFocusedButtonIndex != -1)
	{
		String name = mButtons[mFocusedButtonIndex]->getId();

		if (name == "PlayButton")
		{
			mGame->getGameStateManager().setState("ControlState");
		}
		else if (name == "RetryButton")
		{
			mGame->getGameStateManager().setState("MainState");
		}
		else if (name == "QuitButton")
		{
			mGame->getWindow().close();
		}
		else if (name == "MenuButton")
		{
			mGame->getGameStateManager().setState("MenuState");
		}
	}



	if (mTimer < mNextButtonTimer)
	{
		mTimer += deltaTime;
	}
	else
	{
		float position = InputManager::getInstance().getAxisPosition("DPad", 0);

		if (position != 0.f && !mMovedLastFrame)
		{
			if (position < 0.f)
			{
				updateFocusedButtonIndex(1);
			}
			if (position > 0.f)
			{
				updateFocusedButtonIndex(-1);
			}
			mButtons[mFocusedButtonIndex]->get_component<SoundComponent>()->setSound("Change");
		}
		mTimer = 0.f;
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
