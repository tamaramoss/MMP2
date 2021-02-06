#pragma once

#include <string>

#include "GameObjectManager.h"
#include "GuiManager.h"

#include "GameStateManager.h"

class InputManager;
class DebugDraw;
class GuiManager;

class Game
{
public:
	struct Config
	{
		Vector2i mResolution{1280, 720};
		std::string mWindowName = "Big Scary Escapetuary";
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
	GameObjectManager* mGameObjectManager;

	GuiManager* mTGuiWrapper = nullptr;
	InputManager* mInputManager = nullptr;
	DebugDraw* mDebugDraw = nullptr;
};
