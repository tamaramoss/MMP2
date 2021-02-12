/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "GameStateManager.h"
#include <utility>
#include "GameState.h"
#include "Debug.h"
#include "MainState.h"

void GameStateManager::registerState(const std::string& name, GameState::ptr state)
{
	mStates[name] = std::move(state);
}

void GameStateManager::setState(const std::string& stateName)
{
	GameState* state = findState(stateName);
	FF_ASSERT_MSG(state != nullptr, "State could not be found");

	mFutureState = state;
}

void GameStateManager::changeState(GameState* state)
{
	if (state != mCurrentState)
	{
		if (mCurrentState != nullptr)
			mCurrentState->exit();

		mCurrentState = state;

		if (mCurrentState != nullptr)
			mCurrentState->init();
	}
}

void GameStateManager::update(const float deltaTime)
{
	if (mFutureState != nullptr)
	{
		changeState(mFutureState);
		mFutureState = nullptr;
	}

	if (mCurrentState != nullptr)
		mCurrentState->update(deltaTime);
}

void GameStateManager::draw() const
{
	if (mCurrentState != nullptr)
		mCurrentState->draw();
}

void GameStateManager::shutdown() const
{
	mCurrentState->exit();
}

GameState* GameStateManager::findState(const std::string& stateName)
{
	const auto state = mStates.find(stateName);
	if (state != mStates.end())
		return state->second.get();
	return nullptr;
}
