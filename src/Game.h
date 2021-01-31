#pragma once

#include <string>
#include <TGUI/TGUI.hpp>
#include "GameStateManager.h"

class InputManager;
class DebugDraw;

class Game
{
public:
	struct Config
	{
		Vector2i mResolution{1280, 720};
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
	tgui::Gui mGUI;
	GameStateManager mGameStateManager;

	InputManager* mInputManager = nullptr;
	DebugDraw* mDebugDraw = nullptr;
};
