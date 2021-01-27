#pragma once

#include <string>

#include "GameStateManager.h"

class InputManager;
class DebugDraw;

class Game
{
public:
	struct Config
	{
		Vector2i mResolution{800, 600};
		std::string mWindowName = "FinalFrontier";
	};

	Config& getConfig() { return mConfig; }
	RenderWindow& getWindow() { return mWindow; }

	void run();

private:

	bool init();
	void update();
	void draw();
	static void shutdown();

	Config mConfig;

	RenderWindow mWindow;
	GameStateManager mGameStateManager;

	InputManager* mInputManager = nullptr;
	DebugDraw* mDebugDraw = nullptr;
};
