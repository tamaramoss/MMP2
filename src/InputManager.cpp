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
		mEventKeyboardFrame.mKeys[event.key.code] = true;
		break;
	case Event::KeyReleased:
		mEventKeyboardFrame.mKeys[event.key.code] = false;
		break;
	case Event::JoystickButtonPressed:
		mEventJoystickFrame.mButtons[event.joystickButton.button] = true;
		break;
	case Event::JoystickButtonReleased:
		mEventJoystickFrame.mButtons[event.joystickButton.button] = false;
		break;
	default: 
		break;
	}
}

void
InputManager::update()
{
	mLastKeyboardFrame = mCurrentKeyboardFrame;
	mCurrentKeyboardFrame = mEventKeyboardFrame;

	mLastJoystickFrame = mCurrentJoystickFrame;
	mCurrentJoystickFrame = mEventJoystickFrame;
}

void
InputManager::bindKey(const std::string& action, const int key_code, const int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < mPlayerCount, "player out of bounds");
	mActionBinding[playerIdx][action] = key_code;
}

void
InputManager::unbindKey(const std::string& action, const int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < mPlayerCount, "player out of bounds");
	mActionBinding[playerIdx].erase(action);
}

void InputManager::bindButton(const std::string& action, int buttonIdx, int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < mPlayerCount, "player out of bounds");
	mActionBinding[playerIdx][action] = buttonIdx;
}

void InputManager::unbindButton(const std::string& action, int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < mPlayerCount, "player out of bounds");
	mActionBinding[playerIdx].erase(action);
}

int
InputManager::getKeyForAction(const std::string& action, const int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < mPlayerCount, "player out of bounds");

	const auto it = mActionBinding[playerIdx].find(action);
	if (it != mActionBinding[playerIdx].end())
	{
		return it->second;
	}
	return 0;
}

int InputManager::getButtonForAction(const std::string& action, int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < mPlayerCount, "player out of bounds");

	const auto it = mActionBinding[playerIdx].find(action);
	if (it != mActionBinding[playerIdx].end())
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



bool InputManager::isButtonDown(const std::string& action, int playerIdx)
{
	return isButtonDown(getButtonForAction(action, playerIdx));
}

bool InputManager::isButtonUp(const std::string& action, int playerIdx)
{
	return isButtonUp(getKeyForAction(action, playerIdx));
}

bool InputManager::isButtonPressed(const std::string& action, int playerIdx)
{
	return isButtonPressed(getKeyForAction(action, playerIdx));
}

bool InputManager::isButtonReleased(const std::string& action, int playerIdx)
{
	return isButtonReleased(getKeyForAction(action, playerIdx));
}

sf::Vector2f InputManager::getAxisPosition(bool rightJoystick, int playerIdx)
{
	float deadZone = 15.0f;
	
	float xAxis, yAxis;
	if (rightJoystick)
	{
		xAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
		yAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R);
	}
	else
	{
		xAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
		yAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
	}

	if (xAxis < deadZone && xAxis > -deadZone) xAxis = 0.0f;
	if (yAxis < deadZone && yAxis > -deadZone) yAxis = 0.0f;

	const sf::Vector2<float> joystickPosition{ xAxis, yAxis };

	float length = MathUtil::length(joystickPosition);

	
	return length == 0.0f ? joystickPosition : joystickPosition / length;
}

sf::Vector2f InputManager::getLeftJoystickPosition(int playerIdx)
{
	const float deadZone = 30.0f;

	float xAxis = sf::Joystick::getAxisPosition(playerIdx, sf::Joystick::Axis::X);
	float yAxis = sf::Joystick::getAxisPosition(playerIdx, sf::Joystick::Axis::Y);

	if (xAxis < deadZone && xAxis > -deadZone) xAxis = 0.0f;
	if (yAxis < deadZone && yAxis > -deadZone) yAxis = 0.0f;

	const sf::Vector2<float> joystickPosition{ xAxis, yAxis };
	return joystickPosition / MathUtil::length(joystickPosition);
}

sf::Vector2f InputManager::getRightJoystickPosition(int playerIdx)
{
	float deadZone = 30.0f;

	float xAxis = sf::Joystick::getAxisPosition(playerIdx, sf::Joystick::Axis::U);
	float yAxis = sf::Joystick::getAxisPosition(playerIdx, sf::Joystick::Axis::R);


	if (xAxis < deadZone && xAxis > -deadZone) xAxis = 0.0f;
	if (yAxis < deadZone && yAxis > -deadZone) yAxis = 0.0f;

	const sf::Vector2<float> joystickPosition{ xAxis, yAxis };
	return joystickPosition / MathUtil::length(joystickPosition);
}

sf::Vector2f InputManager::getDpadPosition (int playerIdx)
{
	float deadZone = 15.0f;
	
	float xAxis = sf::Joystick::getAxisPosition(playerIdx, sf::Joystick::Axis::PovX);
	float yAxis = -sf::Joystick::getAxisPosition(playerIdx, sf::Joystick::Axis::PovY);


	if (xAxis < deadZone && xAxis > -deadZone) xAxis = 0.0f;
	if (yAxis < deadZone && yAxis > -deadZone) yAxis = 0.0f;

	const sf::Vector2<float> joystickPosition{ xAxis, yAxis };
	float length = MathUtil::length(joystickPosition);


	return length == 0.0f ? joystickPosition : joystickPosition / length;
}

float InputManager::getTriggerPosition(int playerIdx)
{
	float deadZone = 15.0f;

	float zAxis = sf::Joystick::getAxisPosition(playerIdx, sf::Joystick::Axis::Z);

	if (zAxis < deadZone && zAxis > -deadZone) zAxis = 0.0f;

	return zAxis;
}

Vector2f
InputManager::getMousePosition() const
{
	FF_ASSERT_MSG(mRenderWindow != nullptr, "RenderWindow not set for getMousePosition.");

	const auto p = Mouse::getPosition(*mRenderWindow);
	return {static_cast<float>(p.x), static_cast<float>(p.y)};
}
