#include "stdafx.h"

#include "Game.h"

#include <iostream>



#include "ControlState.h"
#include "InputManager.h"
#include "MenuState.h"
#include "MainState.h"
#include "FinalScreen.h"
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
			else
				mInputManager->process(event);
		}

		update();
		draw();
	}

	shutdown();
}

bool Game::init()
{
	mWindow.create(VideoMode(mConfig.mResolution.x, mConfig.mResolution.y),
		mConfig.mWindowName);

	mInputManager = &InputManager::getInstance();

	controllerSetup();

	mDebugDraw = &DebugDraw::getInstance();

	mGameObjectManager = &GameObjectManager::getInstance();
	mGameObjectManager->init();


	mInputManager->set_renderWindow(&mWindow);

	mGameStateManager.registerState("MenuState", make_shared<MenuState>(&mGameStateManager, this));
	auto m = make_shared<MainState>(&mGameStateManager, this);
	//m->init();
	mGameStateManager.registerState("MainState", m);
	
	mGameStateManager.registerState("ControlState", make_shared<ControlState>(&mGameStateManager, this));
	mGameStateManager.registerState("FinalScreen", make_shared<FinalScreen>(&mGameStateManager, this));

	mGameStateManager.setState("MenuState");

	return true;
}

void Game::update()
{
	// starts the clock
	static Clock clock;
	// restart takes time
	auto fDeltaTime = clock.restart();
	const auto fDeltaTimeSeconds = fDeltaTime.asSeconds();

	// must be first call
	mInputManager->update();

	mGameStateManager.update(fDeltaTimeSeconds);
	
	//mDebugDraw->update(fDeltaTimeSeconds);
}

void Game::draw()
{
	mWindow.clear();
	
	mGameStateManager.draw();

	//mDebugDraw->draw(mWindow);


	mWindow.display();
}

void Game::shutdown()
{
}

void Game::controllerSetup()
{
	sf::Joystick::Identification id = sf::Joystick::getIdentification(0);
	//std::cout << "\nVendor ID: " << id.vendorId << "\nProduct ID: " << id.productId << std::endl;
	sf::String controller("Joystick Use: " + id.name);
	//mWindow.setTitle(controller);

	int controllers = 0;

	for (int i = 0; i < Joystick::Count; i++)
	{
		if (Joystick::isConnected(i)) controllers++;
	}

	mInputManager->setTotalConnectedController(controllers);

	// 0 is left side, 1 is right side
	
	if (controllers == 1)
	{
		if (Joystick::getIdentification(0).vendorId == 1356)
		{
			mInputManager->bindButton("Select", 1, 0);
			mInputManager->bindJoystick("DPad", Joystick::Axis::PovY, 0);


			mInputManager->bindButton("Grab", 4, 0);
			mInputManager->bindButton("Grab", 5, 1);
			mInputManager->bindButton("Pull", 10, 0);
			mInputManager->bindButton("Pull", 11, 1);

			//mInputManager->bindJoystick("LeftJoystick", std::vector<Joystick::Axis> { Joystick::Axis::X, Joystick::Axis::Y }, controllerCount);
			//mInputManager->bindJoystick("RightJoystick", std::vector<Joystick::Axis> { Joystick::Axis::Z, Joystick::Axis::R }, controllerCount);

			mInputManager->bindJoystick("JoystickX", Joystick::Axis::X, 0);
			mInputManager->bindJoystick("JoystickY",  Joystick::Axis::Y , 0);
			mInputManager->bindJoystick("JoystickX", Joystick::Axis::Z, 1);
			mInputManager->bindJoystick("JoystickY", Joystick::Axis::R, 1);
		}
		else if (Joystick::getIdentification(0).vendorId == 1118)
		{
			// 0 is left side, 1 is right side

			mInputManager->bindButton("Select", 0, 0);

			mInputManager->bindButton("Grab", 4, 0);
			mInputManager->bindButton("Grab", 5, 1);
			mInputManager->bindButton("Pull", 8, 0);
			mInputManager->bindButton("Pull", 9, 1);

			//mInputManager->bindJoystick("LeftJoystick", std::vector<Joystick::Axis> { Joystick::Axis::X, Joystick::Axis::Y }, controllerCount);
			//mInputManager->bindJoystick("RightJoystick", std::vector<Joystick::Axis> { Joystick::Axis::Z, Joystick::Axis::R }, controllerCount);

			mInputManager->bindJoystick("JoystickX", Joystick::Axis::X, 0);
			mInputManager->bindJoystick("DPad", Joystick::Axis::PovY, 0);
			mInputManager->bindJoystick("JoystickY",  Joystick::Axis::Y , 0);
			mInputManager->bindJoystick("JoystickX", Joystick::Axis::U, 1);
			mInputManager->bindJoystick("JoystickY", Joystick::Axis::V, 1);
		}

		return;
	}

	int controllerCount = 0;
	for (int i = 0; i < controllers; i++)
	{
		if (controllerCount == 2) break;
		
		if (!Joystick::isConnected(i)) continue;

		if (Joystick::getIdentification(i).vendorId == 1356)
		{
			if (controllerCount == 1)
			{
				mInputManager->bindJoystick("JoystickX",  Joystick::Axis::Z, controllerCount);
				mInputManager->bindJoystick("JoystickY", Joystick::Axis::R, controllerCount);
				mInputManager->bindButton("Grab", 5, controllerCount);
				mInputManager->bindButton("Pull", 11, controllerCount);
			}
			else
			{
				mInputManager->bindJoystick("JoystickX", Joystick::Axis::X, controllerCount);
				mInputManager->bindJoystick("JoystickY", Joystick::Axis::Y, controllerCount);
				mInputManager->bindButton("Grab", 4, controllerCount);
				mInputManager->bindButton("Pull", 10, controllerCount);
				mInputManager->bindButton("Select", 1, controllerCount);
				mInputManager->bindJoystick("DPad", Joystick::Axis::PovY, controllerCount);

			}

			//mInputManager->bindJoystick("LeftJoystick", std::vector<Joystick::Axis> { Joystick::Axis::X, Joystick::Axis::Y }, controllerCount);
			//mInputManager->bindJoystick("RightJoystick", std::vector<Joystick::Axis> { Joystick::Axis::Z, Joystick::Axis::R }, controllerCount);

			controllerCount++;
		}
		else if (Joystick::getIdentification(i).vendorId == 1118)
		{
			if (controllerCount == 1)
			{
				mInputManager->bindJoystick("JoystickX", Joystick::Axis::U, controllerCount);
				mInputManager->bindJoystick("JoystickY", Joystick::Axis::V, controllerCount);
				mInputManager->bindButton("Grab", 5, controllerCount);
				mInputManager->bindButton("Pull", 9, controllerCount);
			}
			else
			{
				mInputManager->bindJoystick("JoystickX", Joystick::Axis::X, controllerCount);
				mInputManager->bindJoystick("JoystickY", Joystick::Axis::Y, controllerCount);
				mInputManager->bindButton("Grab", 4, controllerCount);
				mInputManager->bindButton("Pull", 8, controllerCount);

				mInputManager->bindButton("Select", 0, controllerCount);
				mInputManager->bindJoystick("DPad", Joystick::Axis::PovY, controllerCount);

			}

			//mInputManager->bindJoystick("LeftJoystick", std::vector<Joystick::Axis> { Joystick::Axis::X, Joystick::Axis::Y }, controllerCount);
			//mInputManager->bindJoystick("RightJoystick", std::vector<Joystick::Axis> { Joystick::Axis::Z, Joystick::Axis::R }, controllerCount);
		}

		
	}
}
