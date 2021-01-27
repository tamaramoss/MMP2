#include "stdafx.h"

#include "GameStateManager.h"
#include <utility>
#include "GameState.h"
#include "Debug.h"

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

GameState* GameStateManager::findState(const std::string& stateName)
{
	const auto state = mStates.find(stateName);
	if (state != mStates.end())
		return state->second.get();
	return nullptr;
}
