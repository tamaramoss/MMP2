#pragma once

#include <unordered_map>

#include "SFML/Window.hpp"

#include "VectorAlgebra2D.h"

class InputManager
{
public:

	static InputManager& getInstance();

	/// Must be called at beginning of game update, before the update
	/// method. The events are
	/// classified and processed internally.
	/// \param event a single event coming from the sf polling.
	void process(const Event& event);

	/// Must be called after all events have been processed.
	void update();

#pragma region Keyboard
	
	/// Binds an action to the given keycode and for the given player
	/// \param action the name of the action (eg. left, jump, ...)
	/// \param key_code the keycode of the action
	/// \param player_idx the idx of the player
	void bindKey(const std::string& action, int key_code, int playerIdx = 0);

	/// Unbinds an action.
	void unbindKey(const std::string& action, int playerIdx = 0);

	/// Returns true if the key button is currently down.
	bool isKeyDown(const int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return mCurrentKeyboardFrame.mKeys[keyCode];
	}

	/// Returns true if the key button is currently up.
	bool isKeyUp(const int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return !mCurrentKeyboardFrame.mKeys[keyCode];
	}

	/// Returns true if the key button has been pressed.
	bool isKeyPressed(const int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return mCurrentKeyboardFrame.mKeys[keyCode] && !mLastKeyboardFrame.mKeys[keyCode];
	}

	/// Returns true if the key button has been released.
	bool isKeyReleased(const int key_code)
	{
		FF_ASSERT_MSG(key_code >= 0 && key_code < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return !mCurrentKeyboardFrame.mKeys[key_code] && mLastKeyboardFrame.mKeys[key_code];
	}

	/// Returns true if the button for the given Action is currently down.
	bool isKeyDown(const std::string& action, int player_idx = 0);

	/// Returns true if the button for the given Action is currently up.
	bool isKeyUp(const std::string& action, int player_idx = 0);

	/// Returns true if the button for the given Action has been pressed.
	bool isKeyPressed(const std::string& action, int player_idx = 0);

	/// Returns true if the button for the given Action has been released.
	bool isKeyReleased(const std::string& action, int player_idx = 0);

#pragma endregion auch ok

#pragma region Controller
	/// Binds an action to the given keycode and for the given player
	/// \param action the name of the action (eg. left, jump, ...)
	/// \param button_idx the keycode of the action
	/// \param playerIdx the idx of the player
	void bindButton(const std::string& action, int button_idx, int playerIdx = 0);

	/// Unbinds an action.
	void unbindButton(const std::string& action, int playerIdx = 0);

	/// Returns true if the key button is currently down.
	bool isButtonDown(const int buttonCode)
	{
		FF_ASSERT_MSG(buttonCode >= 0 && buttonCode < sf::Joystick::ButtonCount, "KeyCode out of bounds");
		return mCurrentJoystickFrame.mButtons[buttonCode];
	}

	/// Returns true if the key button is currently up.
	bool isButtonUp(const int buttonCode)
	{
		FF_ASSERT_MSG(buttonCode >= 0 && buttonCode < sf::Joystick::ButtonCount, "KeyCode out of bounds");
		return mCurrentJoystickFrame.mButtons[buttonCode];
	}

	/// Returns true if the key button has been pressed.
	bool isButtonPressed(const int buttonCode)
	{
		FF_ASSERT_MSG(buttonCode >= 0 && buttonCode < sf::Joystick::ButtonCount, "KeyCode out of bounds");
		return mCurrentJoystickFrame.mButtons[buttonCode] && !mLastJoystickFrame.mButtons[buttonCode];
	}

	/// Returns true if the key button has been released.
	bool isButtonReleased(const int buttonCode)
	{
		FF_ASSERT_MSG(buttonCode >= 0 && buttonCode < sf::Joystick::ButtonCount, "KeyCode out of bounds");
		return mCurrentJoystickFrame.mButtons[buttonCode] && mLastJoystickFrame.mButtons[buttonCode];
	}

	/// Returns true if the button for the given Action is currently down.
	bool isButtonDown(const std::string& action, int playerIdx = 0);

	/// Returns true if the button for the given Action is currently up.
	bool isButtonUp(const std::string& action, int playerIdx = 0);

	/// Returns true if the button for the given Action has been pressed.
	bool isButtonPressed(const std::string& action, int playerIdx = 0);

	/// Returns true if the button for the given Action has been released.
	bool isButtonReleased(const std::string& action, int playerIdx = 0);

	// Returns position of right or left joystick
	// \param rightJoystick if true -> right joystick , false -> left joystick
	sf::Vector2f getAxisPosition(bool rightJoystick, int playerIdx);

	sf::Vector2f getLeftJoystickPosition(int playerIdx);
	
	sf::Vector2f getRightJoystickPosition(int playerIdx);
	sf::Vector2f getDpadPosition(int playerIdx);
	float getTriggerPosition(int playerIdx);
#pragma endregion good region

	
	/// Returns the current mouse position relative to the window
	Vector2f getMousePosition() const;

	void set_renderWindow(RenderWindow* window)
	{
		mRenderWindow = window;
	}

private:
	InputManager() = default;
	~InputManager() = default;

	InputManager(const InputManager& rhv) = delete;
	InputManager& operator=(const InputManager& rhv) = delete;

	int getKeyForAction(const std::string& action, int playerIdx);
	int getButtonForAction(const std::string& action, int playerIdx);

	struct KeyboardFrameData
	{
		bool mKeys[Keyboard::KeyCount];
	};

	struct JoystickFrameData
	{
		bool mButtons[Joystick::ButtonCount];
	};

	KeyboardFrameData mLastKeyboardFrame{};
	KeyboardFrameData mCurrentKeyboardFrame{};
	KeyboardFrameData mEventKeyboardFrame{};

	JoystickFrameData mLastJoystickFrame{};
	JoystickFrameData mCurrentJoystickFrame{};
	JoystickFrameData mEventJoystickFrame{};

	RenderWindow* mRenderWindow;

	/// maximum allowed players. Can be increased if needed.
	static const int mPlayerCount = 4;
	std::unordered_map<std::string, int> mActionBinding[mPlayerCount];
};
