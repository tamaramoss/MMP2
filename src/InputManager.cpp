#include "stdafx.h"

#include "InputManager.h"

InputManager& InputManager::getInstance()
{
	static InputManager instance;
	return instance;
}

void
InputManager::process(const Event& event)
{
	switch (event.type)
	{
	case Event::KeyPressed:
		m_eventFrame.m_keys[event.key.code] = true;
		break;
	case Event::KeyReleased:
		m_eventFrame.m_keys[event.key.code] = false;
		break;
	default: ;
	}
}

void
InputManager::update()
{
	m_lastFrame = m_currentFrame;
	m_currentFrame = m_eventFrame;
}

void
InputManager::bind(const std::string& action, const int key_code, const int player_idx)
{
	FF_ASSERT_MSG(player_idx < playerCount, "player out of bounds");
	m_actionBinding[player_idx][action] = key_code;
}

void
InputManager::unbind(const std::string& action, const int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < playerCount, "player out of bounds");
	m_actionBinding[playerIdx].erase(action);
}

int
InputManager::getKeyForAction(const std::string& action, const int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < playerCount, "player out of bounds");

	const auto it = m_actionBinding[playerIdx].find(action);
	if (it != m_actionBinding[playerIdx].end())
	{
		return it->second;
	}
	return 0;
}

bool
InputManager::isKeyDown(const std::string& action, const int player)
{
	return isKeyDown(getKeyForAction(action, player));
}

bool
InputManager::isKeyUp(const std::string& action, const int player)
{
	return isKeyUp(getKeyForAction(action, player));
}

bool
InputManager::isKeyPressed(const std::string& action, const int player)
{
	return isKeyPressed(getKeyForAction(action, player));
}

bool
InputManager::isKeyReleased(const std::string& action, const int player)
{
	return isKeyReleased(getKeyForAction(action, player));
}

Vector2f
InputManager::getMousePosition() const
{
	FF_ASSERT_MSG(m_renderWindow != nullptr, "RenderWindow not set for getMousePosition.");

	const auto p = Mouse::getPosition(*m_renderWindow);
	return {static_cast<float>(p.x), static_cast<float>(p.y)};
}
