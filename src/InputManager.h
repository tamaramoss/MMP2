#pragma once

#include <unordered_map>

#include "SFML/Window.hpp"

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

	/// Binds an action to the given keycode and for the given player
	/// \param action the name of the action (eg. left, jump, ...)
	/// \param key_code the keycode of the action
	/// \param player_idx the idx of the player
	void bind(const std::string& action, int key_code, int player_idx = 0);

	/// Unbinds an action.
	void unbind(const std::string& action, int player_idx = 0);

	/// Returns true if the key button is currently down.
	bool isKeyDown(const int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return m_currentFrame.m_keys[keyCode];
	}

	/// Returns true if the key button is currently up.
	bool isKeyUp(const int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return !m_currentFrame.m_keys[keyCode];
	}

	/// Returns true if the key button has been pressed.
	bool isKeyPressed(const int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return m_currentFrame.m_keys[keyCode] && !m_lastFrame.m_keys[keyCode];
	}

	/// Returns true if the key button has been released.
	bool isKeyReleased(const int key_code)
	{
		FF_ASSERT_MSG(key_code >= 0 && key_code < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return !m_currentFrame.m_keys[key_code] && m_lastFrame.m_keys[key_code];
	}

	/// Returns true if the button for the given Action is currently down.
	bool isKeyDown(const std::string& action, int player_idx = 0);

	/// Returns true if the button for the given Action is currently up.
	bool isKeyUp(const std::string& action, int player_idx = 0);

	/// Returns true if the button for the given Action has been pressed.
	bool isKeyPressed(const std::string& action, int player_idx = 0);

	/// Returns true if the button for the given Action has been released.
	bool isKeyReleased(const std::string& action, int player_idx = 0);

	/// Returns the current mouse position relative to the window
	Vector2f getMousePosition() const;

	void set_renderWindow(RenderWindow* window)
	{
		m_renderWindow = window;
	}

private:
	InputManager() = default;
	~InputManager() = default;

	InputManager(const InputManager& rhv) = delete;
	InputManager& operator=(const InputManager& rhv) = delete;

	int getKeyForAction(const std::string& action, int player_idx);

	struct FrameData
	{
		bool m_keys[Keyboard::KeyCount];
	};

	FrameData m_lastFrame{};
	FrameData m_currentFrame{};
	FrameData m_eventFrame{};

	RenderWindow* m_renderWindow;

	/// maximum allowed players. Can be increaded if needed.
	static const int playerCount = 4;
	std::unordered_map<std::string, int> m_actionBinding[playerCount];
};
