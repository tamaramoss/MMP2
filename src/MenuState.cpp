#include "stdafx.h"

#include "MenuState.h"

#include "Game.h"
#include "GameStateManager.h"

#include "InputManager.h"

using namespace std;

void
MenuState::init()
{
	if (mIsInit)
		return;

	//if (!mFont.loadFromFile("../assets/arial.ttf"))
	//{
	//	err() << "Could not load font\n";
	//	return;
	//}
	//mText.setPosition(mGame->getWindow().getView().getCenter());
	//mText.setString("Press space to continue");
	//mText.setFillColor(Color::White);
	//mText.setFont(mFont);
	//mText.setOrigin(
	//	mText.getLocalBounds().width * 0.5f,
	//	mText.getLocalBounds().height * 0.5f);

	mView = mGame->getWindow().getView();
	
	setupGUI();
	
	mIsInit = true;
}

void MenuState::update(float delta)
{
	mGame->getWindow().setView(mView);

	//if (InputManager::getInstance().isButtonPressed("Select"))
	//	mGameStateManager->setState("MainState");
}

void MenuState::draw()
{

}

void MenuState::setupGUI ()
{
	mGame->getGuiWrapper().setGuiStatus(true);
	
	/*GUI->setTarget(mGame->getWindow());*/
	mGame->getGui().add(tgui::Picture::create("../assets/spaceMenu.png"));

	/*tgui::Theme mTheme( "../externalLibs/tgui/themes/Black.txt" );*/
	mTheme.load("../externalLibs/tgui/themes/Black.txt");

	auto button = tgui::Button::create();
	button->setRenderer(mTheme.getRenderer("Button"));
	button->setPosition(mGame->getWindow().getSize().x / 2 - 75.f, mGame->getWindow().getSize().y / 2 - 80.f);
	button->setText("Start");
	button->setSize(150, 60);
	button->connect("pressed", [&]() { mGameStateManager->setState("MainState"); });
	button->setFocused(true);
	//button->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(500));
	mGame->getGuiWrapper().addButton(button, true);


	button = tgui::Button::create();
	button->setRenderer(mTheme.getRenderer("Button"));
	button->setPosition(mGame->getWindow().getSize().x / 2 - 75.f, mGame->getWindow().getSize().y / 2 + 80.f);
	button->setText("Exit");
	button->setSize(150, 60);
	button->connect("pressed", [&]() { mGame->getWindow().close(); });
	//button->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(500));

	mGame->getGuiWrapper().addButton(button, false);
}

void MenuState::exit()
{
	mGame->getGui().removeAllWidgets();
	mGame->getGuiWrapper().setGuiStatus(false);
}
