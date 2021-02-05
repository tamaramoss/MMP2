#include "stdafx.h"

#include "MainState.h"
#include "Game.h"
#include "InputManager.h"

#include "CameraRenderComponent.h"

#include "NLTmxMap.h"
#include "TileMapLoader.h"
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
	mGameObjectManager.init();
	mSpriteManager.init();
	mPhysicsManager.init();

	// load tile map/level
	
	const std::string& resourcePath = "../assets/";
	const auto tilemap = NLLoadTmxMap(resourcePath + "level_smol.tmx");
	FF_ASSERT_MSG(tilemap != nullptr, "Could not load tilemap " + resourcePath + "game.tmx");

	loadTileLayers(tilemap, resourcePath, mSpriteManager);
	loadObjectLayers(tilemap, resourcePath, mSpriteManager);

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

		auto follow = make_shared<PositionFollowComponent>(*camera, mGameObjectManager.getGameObject("Player")->getPosition());
		camera->add_component(follow);
		camera->init();

		mGameObjectManager.addGameObject(camera);
		mSpriteManager.setCamera(render_comp.get());
		//camera->setPosition(Vector2f(192*13,192*90)); // set position of cam
	}

	// Define layer order manually here. Could come from custom file settings.
	mSpriteManager.setLayerOrder({"Floor", "Background", "BackgroundExtras", "BehindObjects", "Walls", "GameObjects", "Top"});

	// spitter player refs
	auto gos = mGameObjectManager.getGameObjects();
	for (auto o : gos)
	{
		if (o.second->getTag() == "Spitter")
		{
			o.second->get_component<SpitterComponent>()->setPlayer(mGameObjectManager.getGameObject("Player"));
		}
	}

	
	if (!mMusic.openFromFile(resourcePath + "Sounds/background.wav"))
		return;

	mMusic.setLoop(true);
	mMusic.play();
	mMusic.setVolume(50);
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
	{
		if (go_pair.second->isMarkedForDelete())
			mGameObjectManager.removeGameObject(go_pair.first);
		else
			go_pair.second->update(deltaTime);

	}

	// set camera to player + hands + tiny offset
	auto player = mGameObjectManager.getGameObject("Player");
	auto spriteBounds = player->get_component<AnimationComponent>()->getCurrentAnimation()->getSprite().getLocalBounds();
	auto playerPos = player->getPosition() - sf::Vector2f(spriteBounds.width, 0);
	auto handPos = (mGameObjectManager.getGameObject("Hand0")->getPosition() + mGameObjectManager.getGameObject("Hand1")->getPosition()) / 2.f;

	Vector2f position = (playerPos + handPos) / 2.f + sf::Vector2f(0, -750.f);

	mGameObjectManager.getGameObject("Camera")->get_component<PositionFollowComponent>()->setFollowPosition(position);

	if (player->get_component<PlayerBodyComponent>()->isPlayerDead())
	{
		auto m = mGameObjectManager.getGameObject("Dead");
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
		auto m = mGameObjectManager.getGameObject("Win");
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
	mGameObjectManager.shutdown();
}
