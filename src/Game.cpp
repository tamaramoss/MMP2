#include "stdafx.h"

#include "Game.h"

#include <iostream>


#include "InputManager.h"
#include "MenuState.h"
#include "MainState.h"
#include "FinalScreen.h"
#include "DebugDraw.h"
#include "TestState.h"

using namespace std;




void Game::run()
{
	if (!init())
		return;

	while (mWindow.isOpen())
	{
		// process events in the input manager
		Event event{};

		//get information about the joystick
		sf::Joystick::Identification id = sf::Joystick::getIdentification(0);
		//std::cout << "\nVendor ID: " << id.vendorId << "\nProduct ID: " << id.productId << std::endl;
		sf::String controller("Joystick Use: " + id.name);
		mWindow.setTitle(controller);//easily tells us what controller is connected

		//	//query joystick for settings if it's plugged in...
		//if (sf::Joystick::isConnected(0)) {
		//	// check how many buttons joystick number 0 has
		//	unsigned int buttonCount = sf::Joystick::getButtonCount(0);

		//	// check if joystick number 0 has a Z axis
		//	bool hasZ = sf::Joystick::hasAxis(0, sf::Joystick::Z);
		//	bool hasX = sf::Joystick::hasAxis(0, sf::Joystick::X);
		//	bool hasY = sf::Joystick::hasAxis(0, sf::Joystick::Y);
		//	bool hasR = sf::Joystick::hasAxis(0, sf::Joystick::R);
		//	bool hasU = sf::Joystick::hasAxis(0, sf::Joystick::U);
		//	bool hasV = sf::Joystick::hasAxis(0, sf::Joystick::V);
		//	bool hasPovX = sf::Joystick::hasAxis(0, sf::Joystick::PovX);
		//	bool hasPovY = sf::Joystick::hasAxis(0, sf::Joystick::PovY);

		//	std::cout << "Button count: " << buttonCount << std::endl;
		//	std::cout << "Has a z-axis: " << hasZ << std::endl;
		//	std::cout << "Has a x-axis: " << hasX << std::endl;
		//	std::cout << "Has a y-axis: " << hasY << std::endl;
		//	std::cout << "Has a r-axis: " << hasR << std::endl;
		//	std::cout << "Has a u-axis: " << hasU << std::endl;
		//	std::cout << "Has a v-axis: " << hasV << std::endl;
		//	std::cout << "Has a Povx-axis: " << hasPovX << std::endl;
		//	std::cout << "Has a PovY-axis: " << hasPovY << std::endl;
		//}
		
		while (mWindow.pollEvent(event))
		{
			if (event.type == Event::Closed)
				mWindow.close();
			else
				mInputManager->process(event);

			if (event.type == sf::Event::JoystickMoved || event.type == sf::Event::JoystickButtonPressed)
				mTGuiWrapper->process(event);

		}

		update();
		draw();
	}

	shutdown();
}

bool Game::init()
{
	//
	mWindow.create(VideoMode(mConfig.mResolution.x, mConfig.mResolution.y),
		mConfig.mWindowName);

	mInputManager = &InputManager::getInstance();

	//mInputManager->bindKey("Exit", Keyboard::Escape);
	//mInputManager->bindKey("Select", Keyboard::Space);

	//// May move to view later on
	//mInputManager->bindKey("up", Keyboard::W, 1);
	//mInputManager->bindKey("left", Keyboard::A, 1);
	//mInputManager->bindKey("down", Keyboard::S, 1);
	//mInputManager->bindKey("right", Keyboard::D, 1);

	//mInputManager->bindKey("up", Keyboard::Up, 0);
	//mInputManager->bindKey("left", Keyboard::Left, 0);
	//mInputManager->bindKey("down", Keyboard::Down, 0);
	//mInputManager->bindKey("right", Keyboard::Right, 0);

	//mInputManager->bindKey("debug draw", Keyboard::F1, 0);

	mInputManager->bindButton("up", 3, 0);
	mInputManager->bindButton("left", 0, 0);
	mInputManager->bindButton("right", 2, 0);
	mInputManager->bindButton("Select", 1);

	mInputManager->bindButton("GrabLeft", 4);
	mInputManager->bindButton("GrabRight", 5);
	mInputManager->bindButton("PullLeft", 6);
	mInputManager->bindButton("PullRight", 7);

	mDebugDraw = &DebugDraw::getInstance();

	mTGuiWrapper = &TGuiWrapper::getInstance();
	mTGuiWrapper->setGame(this);

	
	//
	mGameStateManager.registerState("MenuState", make_shared<MenuState>(&mGameStateManager, this));
	mGameStateManager.registerState("MainState", make_shared<MainState>(&mGameStateManager, this));
	mGameStateManager.registerState("FinalScreen", make_shared<FinalScreen>(&mGameStateManager, this));
	mGameStateManager.registerState("TestState", make_shared<TestState>(&mGameStateManager, this));


	mInputManager->set_renderWindow(&mWindow);

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

	mTGuiWrapper->getGui().draw();


	mGameStateManager.draw();

	//mDebugDraw->draw(mWindow);


	mWindow.display();
}

void Game::shutdown()
{
}
