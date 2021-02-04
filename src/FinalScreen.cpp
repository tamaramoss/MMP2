#include "stdafx.h"
//#include "FinalScreen.h"
//
//#include "Game.h"
//#include "GameStateManager.h"
//
//using namespace std;
//
//void
//FinalScreen::init()
//{
//	if (mIsInit)
//		return;
//
//	mView = mGame->getWindow().getView();
//
//	setupGUI();
//
//	mIsInit = true;
//}
//
//void FinalScreen::update(float delta)
//{
//	mGame->getWindow().setView(mView);
//}
//
//void FinalScreen::draw()
//{
//
//}
//
//void FinalScreen::setupGUI()
//{
//	TGuiWrapper::getInstance().getGui().add(tgui::Picture::create("../assets/space.png"));
//
//	tgui::Theme theme("../externalLibs/tgui/themes/Black.txt");
//
//	auto button = tgui::Button::create();
//	button->setRenderer(theme.getRenderer("Button"));
//	button->setPosition(mGame->getWindow().getSize().x / 2 - 75.f, mGame->getWindow().getSize().y / 2 - 80.f);
//	button->setText("Play again");
//	button->setSize(150, 60);
//	button->connect("pressed", [&]() { mGameStateManager->setState("MainState"); });
//	button->setFocused(false);
//	TGuiWrapper::getInstance().addButton(button, false);
//
//	button = tgui::Button::create();
//	button->setRenderer(theme.getRenderer("Button"));
//	button->setPosition(mGame->getWindow().getSize().x / 2 - 75.f, mGame->getWindow().getSize().y / 2 );
//	button->setText("Back to menu");
//	button->setSize(150, 60);
//	button->connect("pressed", [&]() { mGameStateManager->setState("MenuState"); });
//	button->setFocused(false);
//	TGuiWrapper::getInstance().addButton(button, false);
//
//
//	button = tgui::Button::create();
//	button->setRenderer(theme.getRenderer("Button"));
//	button->setPosition(mGame->getWindow().getSize().x / 2 - 75.f, mGame->getWindow().getSize().y / 2 + 80.f);
//	button->setText("Exit");
//	button->setSize(150, 60);
//	button->connect("pressed", [&]() { mGame->getWindow().close(); });
//	TGuiWrapper::getInstance().addButton(button, false);
//}
//
//void FinalScreen::exit()
//{
//	TGuiWrapper::getInstance().exit();
//}
