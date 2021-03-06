/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"
#include "ControlState.h"

#include "CameraRenderComponent.h"
#include "Game.h"
#include "GameStateManager.h"
#include "InputManager.h"
#include "NLTmxMap.h"
#include "TileMapManager.h"

using namespace std;

ControlState::ControlState(GameStateManager* gameStateManager, Game* game)
	: GameState(gameStateManager, game)
	, mSpriteManager(game->getWindow())
{
}

void
ControlState::init()
{
	if (mIsInit)
		return;

	mSpriteManager.init();


	// load tile map/level
	{
		const std::string& resourcePath = "../assets/";
		const auto tilemap = NLLoadTmxMap(resourcePath + "controls.tmx");
		FF_ASSERT_MSG(tilemap != nullptr, "Could not load tilemap " + resourcePath + "game.tmx");

		TileMapManager::getInstance().loadTileLayers(tilemap, resourcePath, mSpriteManager);

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

		GameObjectManager::getInstance().addGameObject(camera);
		mSpriteManager.setCamera(render_comp.get());
		//camera->setPosition(Vector2f(192*13,192*90)); // set position of cam
	}
	mSpriteManager.setLayerOrder({ "Background" });

	mIsInit = true;

}

void ControlState::update(float deltaTime)
{
	if (InputManager::getInstance().isButtonPressed("Select"))
		mGameStateManager->setState("MainState");
}

void ControlState::draw()
{
	mSpriteManager.draw();
}


void ControlState::exit()
{
	mSpriteManager.shutdown();
	GameObjectManager::getInstance().reset();
	
	mIsInit = false;
}
