#include "stdafx.h"
#include "FinalScreen.h"


#include "CameraRenderComponent.h"
#include "Game.h"
#include "GameStateManager.h"
#include "NLTmxMap.h"
#include "TileMapLoader.h"

using namespace std;

FinalScreen::FinalScreen(GameStateManager* gameStateManager, Game* game)
	: GameState(gameStateManager, game)
	, mSpriteManager(game->getWindow())
{
}

void
FinalScreen::init()
{
	if (mIsInit)
		return;
	mGuiManager = std::make_shared<TGuiWrapper>(mGame);
	mGameObjectManager.init();
	mSpriteManager.init();

	// load tile map/level
	{
		const std::string& resourcePath = "../assets/";
		const auto tilemap = NLLoadTmxMap(resourcePath + "end_screen.tmx");
		FF_ASSERT_MSG(tilemap != nullptr, "Could not load tilemap " + resourcePath + "game.tmx");

		loadTileLayers(tilemap, resourcePath, mSpriteManager, mGuiManager.get());
		loadObjectLayers(tilemap, resourcePath, mSpriteManager, mGuiManager.get());

		delete tilemap;
	}

	// Moving camera
	{
		auto v = mGame->getWindow().getView();
		v.setSize(1280, 720);
		v.setCenter(1280 / 2, 720 / 2);
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
	mSpriteManager.setLayerOrder({ "Background", "Buttons" });

	if (!mMusic.openFromFile("../assets/Sounds/background.ogg"))
		return;

	mMusic.setLoop(true);
	mMusic.play();
	mMusic.setVolume(50);


	mIsInit = true;
}

void FinalScreen::update(float deltaTime)
{
	// update remaining game objects
	for (auto go_pair : mGameObjectManager.getGameObjects())
	{
		if (go_pair.second->isMarkedForDelete())
			mGameObjectManager.removeGameObject(go_pair.first);
		else
			go_pair.second->update(deltaTime);
	}

	mGuiManager->process(deltaTime);
}

void FinalScreen::draw()
{
	mSpriteManager.draw();

}

void FinalScreen::exit()
{
	mSpriteManager.shutdown();
	mGameObjectManager.shutdown();
	mGuiManager->exit();

	mMusic.stop();

	mIsInit = false;
}
