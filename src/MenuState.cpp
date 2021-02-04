#include "stdafx.h"

#include "MenuState.h"

#include "CameraRenderComponent.h"
#include "Game.h"
#include "GameStateManager.h"
#include "NLTmxMap.h"
#include "TileMapLoader.h"

using namespace std;

MenuState::MenuState(GameStateManager* gameStateManager, Game* game) 
	: GameState(gameStateManager, game)
	, mSpriteManager(game->getWindow())
{
}

void
MenuState::init()
{
	if (mIsInit)
		return;

	mGuiManager = std::make_shared<TGuiWrapper>(mGame);
	mGameObjectManager.init();
	mSpriteManager.init();

	// load tile map/level
	{
		const std::string& resourcePath = "../assets/";
		const auto tilemap = NLLoadTmxMap(resourcePath + "main_menu.tmx");
		FF_ASSERT_MSG(tilemap != nullptr, "Could not load tilemap " + resourcePath + "game.tmx");

		loadTileLayers(tilemap, resourcePath, mSpriteManager, mGuiManager.get());
		loadObjectLayers(tilemap, resourcePath, mSpriteManager, mGuiManager.get());

		delete tilemap;
	}

	// Moving camera
	{
		auto v = mGame->getWindow().getView();
		v.setSize(1280, 720);
		mGame->getWindow().setView(v);

		auto camera = make_shared<GameObject>("Camera", "Camera");
		auto render_comp = make_shared<CameraRenderComponent>(
			*camera, mGame->getWindow(), mGame->getWindow().getView());
		camera->add_component(render_comp);

		camera->init();

		mGameObjectManager.addGameObject(camera);
		mSpriteManager.setCamera(render_comp.get());
		//camera->setPosition(Vector2f(192*13,192*90)); // set position of cam
	}

	//mView = mGame->getWindow().getView();
	
	mSpriteManager.setLayerOrder({ "Background", "Buttons" });

	
	mIsInit = true;
}

void MenuState::update(float deltaTime)
{
	//mGame->getWindow().setView(mView);

	//EventBus::getInstance().processEvents(deltaTime);

	// update remaining game objects
	for (auto go_pair : mGameObjectManager.getGameObjects())
	{
		if (go_pair.second->isMarkedForDelete())
			mGameObjectManager.removeGameObject(go_pair.first);
		else
			go_pair.second->update(deltaTime);
	}

	mGuiManager->process();

	//if (InputManager::getInstance().isButtonPressed("Select"))
	//	mGameStateManager->setState("MainState");
}

void MenuState::draw()
{
	mSpriteManager.draw();
}

void MenuState::setupGUI ()
{	
	//TGuiWrapper::getInstance().getGui().add(tgui::Picture::create("../assets/spaceMenu.png"));

	//tgui::Theme theme("../externalLibs/tgui/themes/Black.txt");

	//auto button = tgui::Button::create();
	//button->setRenderer(theme.getRenderer("Button"));
	//button->setPosition(mGame->getWindow().getSize().x / 2 - 75.f, mGame->getWindow().getSize().y / 2 - 80.f);
	//button->setText("Start");
	//button->setSize(150, 60);
	//button->connect("pressed", [&]() { mGameStateManager->setState("MainState"); });
	//button->setFocused(false);
	////TGuiWrapper::getInstance().addButton(button, false);


	//button = tgui::Button::create();
	//button->setRenderer(theme.getRenderer("Button"));
	//button->setPosition(mGame->getWindow().getSize().x / 2 - 75.f, mGame->getWindow().getSize().y / 2 + 80.f);
	//button->setText("Exit");
	//button->setSize(150, 60);
	//button->connect("pressed", [&]() { mGame->getWindow().close(); });
	//TGuiWrapper::getInstance().addButton(button, false);
}

void MenuState::exit()
{

	mSpriteManager.shutdown();
	mGameObjectManager.shutdown();
	//TGuiWrapper::getInstance().exit();
	mIsInit = false;
}
