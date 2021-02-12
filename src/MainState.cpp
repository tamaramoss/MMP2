/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "MainState.h"
#include "Game.h"
#include "InputManager.h"

#include "CameraRenderComponent.h"

#include "NLTmxMap.h"
#include "TileMapManager.h"
#include "PositionFollowComponent.h"
#include "AnimationComponent.h"
#include "GameObjectEvents.h"
#include "PlayerBodyComponent.h"
#include "RenderComponentEvents.h"
#include "SpitterComponent.h"

using namespace std;

MainState::MainState(GameStateManager* gameStateManager, Game* game)
	: GameState(gameStateManager, game)
	  , mSpriteManager(game->getWindow())
{
}

void MainState::init()
{
	if (mIsInit)
		return;

	mSpriteManager.init();
	mPhysicsManager.init();

	// load tile map/level
	
	const std::string& resourcePath = "../assets/";
	const auto tilemap = NLLoadTmxMap(resourcePath + "level_smol.tmx");
	FF_ASSERT_MSG(tilemap != nullptr, "Could not load tilemap " + resourcePath + "game.tmx");

	TileMapManager::getInstance().loadTileLayers(tilemap, resourcePath, mSpriteManager);
	TileMapManager::getInstance().loadObjectLayers(tilemap, resourcePath, mSpriteManager);

	delete tilemap;
	

	// Moving camera
	{
		auto v = mGame->getWindow().getView();
		v.setSize(1280 * 5.5f, 720 * 5.5f);
		mGame->getWindow().setView(v);

		auto camera = make_shared<GameObject>("Camera", "Camera");
		auto render_comp = make_shared<CameraRenderComponent>(
			*camera, mGame->getWindow(), mGame->getWindow().getView());
		camera->add_component(render_comp);

		auto follow = make_shared<PositionFollowComponent>(*camera, GameObjectManager::getInstance().getGameObject("Player")->getPosition());
		camera->add_component(follow);
		camera->init();

		GameObjectManager::getInstance().addGameObject(camera);
		mSpriteManager.setCamera(render_comp.get());
		//camera->setPosition(Vector2f(192*13,192*90)); // set position of cam
	}

	// Define layer order manually here. Could come from custom file settings.
	mSpriteManager.setLayerOrder({"Floor", "Background", "BehindObjects", "Walls",  "BackgroundExtras", "GameObjects", "Top"});
	// spitter player refs
	auto gos = GameObjectManager::getInstance().getGameObjects();
	for (auto o : gos)
	{
		if (o.second->getTag() == "Spitter")
		{
			o.second->get_component<SpitterComponent>()->setPlayer(GameObjectManager::getInstance().getGameObject("Player"));
		}
	}
	
	mIsInit = true;
}

void MainState::update(const float deltaTime)
{
	mPhysicsManager.update(deltaTime);
	EventBus::getInstance().processEvents(deltaTime);

	// update remaining game objects
	for (auto go_pair : GameObjectManager::getInstance().getGameObjects())
	{
		if (go_pair.second->isMarkedForDelete())
			GameObjectManager::getInstance().removeGameObject(go_pair.first);
		else
			go_pair.second->update(deltaTime);
	}

	// set camera to player + hands + tiny offset
	auto player = GameObjectManager::getInstance().getGameObject("Player");
	auto spriteBounds = player->get_component<AnimationComponent>()->getCurrentAnimation()->getSprite().getLocalBounds();
	auto playerPos = player->getPosition() - sf::Vector2f(spriteBounds.width, 0);
	auto handPos = (GameObjectManager::getInstance().getGameObject("Hand0")->getPosition() + GameObjectManager::getInstance().getGameObject("Hand1")->getPosition()) / 2.f;

	Vector2f position = (playerPos + handPos) / 2.f + sf::Vector2f(0, -750.f);

	GameObjectManager::getInstance().getGameObject("Camera")->get_component<PositionFollowComponent>()->setFollowPosition(position);

	if (player->get_component<PlayerBodyComponent>()->isPlayerDead())
	{
		auto m = GameObjectManager::getInstance().getGameObject("Dead");
		m->setPosition(Vector2f(position.x, position.y));

		if (mTimer < mNextStateTimer)
		{
			mTimer += deltaTime;
		}
		else
		{
			mGameStateManager->setState("FinalScreen");
		}
	}

	if (player->get_component<PlayerBodyComponent>()->isLevelWon())
	{
		auto m = GameObjectManager::getInstance().getGameObject("Win");
		m->setPosition(Vector2f(position.x, position.y));

		if (mTimer < mNextStateTimer)
		{
			mTimer += deltaTime;
		}
		else
		{
			mGameStateManager->setState("FinalScreen");
		}
	}
}

void MainState::draw()
{
	mSpriteManager.draw();
}

void MainState::exit()
{
	mPhysicsManager.shutdown();
	mSpriteManager.shutdown();
	GameObjectManager::getInstance().reset();
	mTimer = 0.f;

	mIsInit = false;
}
