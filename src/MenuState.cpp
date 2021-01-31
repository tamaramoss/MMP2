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

	if (!mFont.loadFromFile("../assets/arial.ttf"))
	{
		err() << "Could not load font\n";
		return;
	}
	mText.setPosition(mGame->getWindow().getView().getCenter());
	mText.setString("Press space to continue");
	mText.setFillColor(Color::White);
	mText.setFont(mFont);
	mText.setOrigin(
		mText.getLocalBounds().width * 0.5f,
		mText.getLocalBounds().height * 0.5f);

	mView = mGame->getWindow().getView();
	//mGUI.setTarget(mGame->getWindow());
	//const std::string path("Black.txt");

	//tgui::Theme theme(path.c_str());

	//auto progressBar = tgui::ProgressBar::create();
	//progressBar->setRenderer(theme.getRenderer("ProgressBar"));
	//progressBar->setPosition(10, 500);
	//progressBar->setSize(200, 20);
	//progressBar->setValue(50);
	//mGUI.add(progressBar);
	//
	//auto label = tgui::Label::create();
	//label->setRenderer(theme.getRenderer("Label"));
	//label->setText(std::to_string(progressBar->getValue()) + "%");
	//label->setPosition(220, 500);
	//label->setTextSize(18);
	//mGUI.add(label);

	
	mIsInit = true;
}

void MenuState::update(float delta)
{
	mGame->getWindow().setView(mView);

	if (InputManager::getInstance().isButtonPressed("Select"))
		mGameStateManager->setState("MainState");
}

void MenuState::draw()
{
	//mGame->getWindow().draw(mText);

	//mGUI.draw();
}

void MenuState::setupGUI ()
{
	
}
