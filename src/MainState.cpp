#include "stdafx.h"

#include "MainState.h"
#include "Game.h"
#include "InputManager.h"

#include "CameraRenderComponent.h"

#include "NLTmxMap.h"
#include "TileMapLoader.h"

using namespace std;

MainState::MainState(GameStateManager* gameStateManager, Game* game)
	: GameState(gameStateManager, game)
	  , mSpriteManager(game->getWindow())
{
}

void MainState::init()
{
	mGameObjectManager.init();
	mSpriteManager.init();
	mPhysicsManager.init();

	// load tile map/level
	{
		const std::string& resourcePath = "../assets/";
		const auto tilemap = NLLoadTmxMap(resourcePath + "game_test.tmx");
		FF_ASSERT_MSG(tilemap != nullptr, "Could not load tilemap " + resourcePath + "game.tmx");

		loadTileLayers(tilemap, resourcePath, mSpriteManager);
		loadObjectLayers(tilemap, resourcePath, mSpriteManager);

		delete tilemap;
	}

	// Moving camera
	{
		auto v = mGame->getWindow().getView();
		v.setSize(1280 * 5, 720 * 5);
		mGame->getWindow().setView(v);

		auto camera = make_shared<GameObject>("Camera");
		auto render_comp = make_shared<CameraRenderComponent>(
			*camera, mGame->getWindow(), mGame->getWindow().getView());
		camera->add_component(render_comp);
		camera->init();

		mGameObjectManager.addGameObject(camera);
		mSpriteManager.setCamera(render_comp.get());
		camera->setPosition(Vector2f(192*13,192*90)); // set position of cam

		

	}

	// Define layer order manually here. Could come from custom file settings.
	mSpriteManager.setLayerOrder({"Floor", "Background", "GameObjects", "Top"});
}

void MainState::update(const float deltaTime)
{
	if (InputManager::getInstance().isKeyPressed("Exit"))
	{
		mGameStateManager->setState("MenuState");
		return;
	}

	mPhysicsManager.update(deltaTime);
	EventBus::getInstance().processEvents(deltaTime);

	// update remaining game objects
	for (auto go_pair : mGameObjectManager.getGameObjects())
		go_pair.second->update(deltaTime);
}

void MainState::draw()
{
	mSpriteManager.draw();
}

void
MainState::exit()
{
	mPhysicsManager.shutdown();
	mSpriteManager.shutdown();
	mGameObjectManager.shutdown();
}
