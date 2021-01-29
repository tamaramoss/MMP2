#include "stdafx.h"

#include "Game.h"

#include "InputManager.h"
#include "MenuState.h"
#include "MainState.h"
#include "DebugDraw.h"

using namespace std;


void Game::run()
{
	if (!init())
		return;

	while (mWindow.isOpen())
	{
		// process events in the input manager
		Event event{};
		while (mWindow.pollEvent(event))
		{
			if (event.type == Event::Closed)
				mWindow.close();
			else if (event.type == Event::KeyPressed || event.type == Event::KeyReleased)
				mInputManager->process(event);
			else
				mGUI.handleEvent(event);
		}

		update();
		draw();
	}

	shutdown();
}

bool Game::init()
{
	mInputManager = &InputManager::getInstance();

	mInputManager->bind("Exit", Keyboard::Escape);
	mInputManager->bind("Select", Keyboard::Space);

	// May move to view later on
	mInputManager->bind("up", Keyboard::W, 1);
	mInputManager->bind("left", Keyboard::A, 1);
	mInputManager->bind("down", Keyboard::S, 1);
	mInputManager->bind("right", Keyboard::D, 1);

	mInputManager->bind("up", Keyboard::Up, 0);
	mInputManager->bind("left", Keyboard::Left, 0);
	mInputManager->bind("down", Keyboard::Down, 0);
	mInputManager->bind("right", Keyboard::Right, 0);

	mInputManager->bind("debug draw", Keyboard::F1, 0);

	mDebugDraw = &DebugDraw::getInstance();

	//
	mGameStateManager.registerState("MenuState", make_shared<MenuState>(&mGameStateManager, this));
	mGameStateManager.registerState("MainState", make_shared<MainState>(&mGameStateManager, this));

	//
	mWindow.create(VideoMode(mConfig.mResolution.x, mConfig.mResolution.y),
	                mConfig.mWindowName);
	mGUI.setTarget(mWindow);

	mInputManager->set_renderWindow(&mWindow);

	mGameStateManager.setState("MenuState");

	return true;
}

void Game::update()
{
	// starts the clock
	static Clock clock;
	// restart takes time
	auto deltaTime = clock.restart();
	const auto f_deltaTimeSeconds = deltaTime.asSeconds();

	// must be first call
	mInputManager->update();

	mGameStateManager.update(f_deltaTimeSeconds);

	mDebugDraw->update(f_deltaTimeSeconds);
}

void Game::draw()
{
	mWindow.clear();

	mGameStateManager.draw();

	mDebugDraw->draw(mWindow);

	mGUI.draw();

	mWindow.display();
}

void Game::shutdown()
{
}
