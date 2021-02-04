#pragma once

#include <string>
#include "TGuiWrapper.h"

#include "GameStateManager.h"

class InputManager;
class DebugDraw;
class TGuiWrapper;

class Game
{
public:
	struct Config
	{
		Vector2i mResolution{1280, 720};
		std::string mWindowName = "ProjeccRejecc";
	};

	Config& getConfig() { return mConfig; }
	RenderWindow& getWindow() { return mWindow; }

	GameStateManager& getGameStateManager() { return mGameStateManager; }
	
	void run();
private:

	bool init();
	void update();
	void draw();
	static void shutdown();

	void controllerSetup();

	Config mConfig;

	RenderWindow mWindow;
	GameStateManager mGameStateManager;

	TGuiWrapper* mTGuiWrapper = nullptr;
	InputManager* mInputManager = nullptr;
	DebugDraw* mDebugDraw = nullptr;
};
