/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"
#include "FinalScreen.h"


#include "CameraRenderComponent.h"
#include "Game.h"
#include "GameStateManager.h"
#include "NLTmxMap.h"
#include "TileMapManager.h"

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
	mGuiManager = std::make_shared<GuiManager>(mGame);

	mSpriteManager.init();

	// load tile map/level
	{
		const std::string& resourcePath = "../assets/";
		const auto tilemap = NLLoadTmxMap(resourcePath + "end_screen.tmx");
		FF_ASSERT_MSG(tilemap != nullptr, "Could not load tilemap " + resourcePath + "game.tmx");

		TileMapManager::getInstance().loadTileLayers(tilemap, resourcePath, mSpriteManager, mGuiManager.get());
		TileMapManager::getInstance().loadObjectLayers(tilemap, resourcePath, mSpriteManager, mGuiManager.get());

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

		GameObjectManager::getInstance().addGameObject(camera);
		mSpriteManager.setCamera(render_comp.get());
		//camera->setPosition(Vector2f(192*13,192*90)); // set position of cam
	}
	mSpriteManager.setLayerOrder({ "Background", "Buttons" });

	mIsInit = true;
}

void FinalScreen::update(float deltaTime)
{
	// update remaining game objects
	for (auto go_pair : GameObjectManager::getInstance().getGameObjects())
	{
		if (go_pair.second->isMarkedForDelete())
			GameObjectManager::getInstance().removeGameObject(go_pair.first);
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
	GameObjectManager::getInstance().reset();
	mGuiManager->exit();

	mIsInit = false;
}
