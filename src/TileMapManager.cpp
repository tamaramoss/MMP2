#include "stdafx.h"



#include "TileMapManager.h"

#include "TileLayerRenderComponent.h"
#include "SpriteRenderComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

#include "RenderComponentEvents.h"
#include "GameObjectEvents.h"
#include "PhysicsManager.h"
#include <iostream>


#include "AnimationComponent.h"
#include "Animation.h"
#include "PlayerBodyComponent.h"
#include "HandMoveComponent.h"
#include "RockTimedComponent.h"
#include "RockNormalComponent.h"
#include "RockOneTimeComponent.h"
#include "SoundComponent.h"
#include "SpitterComponent.h"
#include "ConstantVelocityComponent.h"

TileMapManager& TileMapManager::getInstance()
{
	static TileMapManager instance;
	return instance;
}

void TileMapManager::loadTileLayers(NLTmxMap* tilemap, const std::string& resourcePath,
	SpriteManager& spriteManager, GuiManager* guiManager)
{
	//err() << "Load tilemap with size: " << tilemap->width << ", "
	//	<< tilemap->height << " and tile-size: " << tilemap->tileWidth
	//	<< ", " << tilemap->tileHeight << std::endl;

	spriteManager.setTileSize({ tilemap->tileWidth, tilemap->tileHeight });

	// load textures for every tileset
	SpriteManager::TileSetMap tileSets;
	for (auto tileset : tilemap->tilesets)
	{
		//err() << "Load tileset: " << tileset->name << " width filename: "
		//	<< tileset->filename << " and tile-size: " << tileset->tileWidth
		//	<< ", " << tileset->tileHeight << std::endl;

		auto texture = make_shared<Texture>();
		if (!texture->loadFromFile(resourcePath + tileset->filename))
			err() << "Could not load texture " << resourcePath + tileset->filename << endl;
		tileSets[tileset->name] = texture;
	}
	spriteManager.setTileSets(tileSets);

	// go through all layers
	std::vector<TileLayer> tile_layers;
	tile_layers.resize(tilemap->layers.size());

	for (auto layer_idx = 0; layer_idx < static_cast<int>(tilemap->layers.size()); layer_idx++)
	{
		const auto layer = tilemap->layers[layer_idx];
		//err() << "Load layer: " << layer->name << " with width: "
		//	<< layer->width << " and height: " << layer->height << std::endl;

		const auto size = layer->width * layer->height;

		tile_layers[layer_idx] = TileLayer{
			layer->name, Vector2i{layer->width, layer->height},
			Vector2i{tilemap->tileWidth, tilemap->tileHeight}
		};

		// go over all elements in the layer
		for (auto i = 0; i < size; i++)
		{
			const auto grid = layer->data[i];

			if (grid == 0)
			{
				// 0 means there is no tile at this grid position.

				// This continue also indicates that the layers are not 
				// represented as two-dimensional arrays, but 
				// only a list of tiles sorted by position from top left
				// to bottom right. (Hint: position could be useful for 
				// quickly finding, which tiles to cull from current 
				// viewport.)					
				continue;
			}

			// get tileset and tileset texture
			const auto tileset = tilemap->get_tileset_for_grid(grid);
			const Vector2i tileSize(tilemap->tileWidth, tilemap->tileHeight);
			auto texture = tileSets[tileset->name];

			assert(texture != nullptr);

			// horizontal tile count in tileset texture
			const int tile_count_x = texture->getSize().x / tileSize.x;

			// calculate position of tile
			Vector2f position;
			position.x = i % layer->width * static_cast<float>(tileSize.x);
			position.y = i / layer->width * static_cast<float>(tileSize.y);
			//position += offset;

			// calculate 2d idx of tile in tileset texture
			const auto idx = grid - tileset->firstGid;
			const auto idx_x = idx % tile_count_x;
			const auto idx_y = idx / tile_count_x;

			// calculate source area of tile in tileset texture
			IntRect source(idx_x * tileSize.x, idx_y * tileSize.y, tileSize.x, tileSize.y);

			// create tile and put it into a layer
			auto sprite = make_shared<Sprite>();
			sprite->setTexture(*texture);
			sprite->setTextureRect(source);
			sprite->setPosition(position.x, position.y);

			tile_layers[layer_idx].tiles.push_back({ i, sprite });
		}
	}

	// Add objects to layers
	auto go = make_shared<GameObject>("TileMap", "Map");
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(go));
	for (auto layer : tile_layers)
	{
		auto layer_comp = make_shared<TileLayerRenderComponent>(
			*go, spriteManager.getWindow(), layer);
		go->add_component(layer_comp);

		EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer.name, *layer_comp));
	}
	go->init();
}

void TileMapManager::loadObjectLayers(NLTmxMap* tilemap, const std::string& resource_path, SpriteManager& sprite_manager, GuiManager* guiManager)
{
	Player player{};


	// go through all object layers
	for (auto group : tilemap->groups)
	{
		// go over all objects per layer
		for (auto object : group->objects)
		{
			const Vector2f position(static_cast<float>(object->x), static_cast<float>(object->y));

			FloatRect bounds(position.x, position.y, static_cast<float>(object->width),
				static_cast<float>(object->height));


			//if (object->type == "PlayerBody")
			//	auto sprite = makePlayer(object, group->name, resource_path, sprite_manager);
			if (object->type == "Collider")
				auto collider = loadCollider(object, group->name, resource_path, sprite_manager);
			if (object->type == "Trigger")
				auto trigger = loadTrigger(object, group->name, resource_path, sprite_manager);
			if (object->type == "Grabbable")
				auto rock = loadRock(object, group->name, resource_path, sprite_manager);
			if (object->type == "Lava")
				auto lava = loadLava(object, group->name, resource_path, sprite_manager);
			if (object->type == "Goal")
				auto lava = loadGoalTrigger(object, group->name, resource_path, sprite_manager);
			if (object->type == "Button")
				auto button = loadButton(object, group->name, resource_path, sprite_manager, guiManager);
			if (object->type == "Player")
				player.PlayerBody = object;
			if (object->type == "PlayerHandLeft")
				player.PlayerHandLeft = object;
			if (object->type == "PlayerHandRight")
				player.PlayerHandRight = object;
			if (object->type == "Spitter")
				mSpitterBody.push_back(object);
			if (object->type == "SpitterTrigger")
				mSpitterTrigger.push_back(object);
		}
		if (group->name == "GameObjects")
		{
			makePlayer(player, group->name, resource_path, sprite_manager);

			setupSpitter(group->name, resource_path, sprite_manager);
			loadDeadOverlay(sprite_manager);
			loadWinOverlay(sprite_manager);
		}
	}


}


GameObject::ptr TileMapManager::createHand(NLTmxMapObject* object, const std::string& layer, GameObject::ptr parent, const int index, const float speed, const Vector2f startPos, float distanceFromStart, SpriteManager& spriteManager)
{
	std::string name = "Hand" + to_string(index);
	auto gameObject = make_shared<GameObject>(name, "Hand");
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(startPos.x + (index == 0 ? -distanceFromStart : distanceFromStart)), static_cast<float>(startPos.y - distanceFromStart / 2));

#pragma region components

	auto animationComponent = makeAnimationComponent(object, gameObject, layer, spriteManager, { "ToOpen", "ToClose" }, "../assets/Sprites/", 0.1f, false, false);

	auto buffer = std::make_shared<sf::SoundBuffer>();
	if (!buffer->loadFromFile("../assets/Sounds/grab.ogg"))
		cout << "No music Here" << endl;

	auto soundComp = std::make_shared<SoundComponent>(*gameObject);
	soundComp->registerSound("Grab", buffer);

	auto buffer2 = std::make_shared<sf::SoundBuffer>();
	if (!buffer2->loadFromFile("../assets/Sounds/pull_up.ogg"))
		cout << "No music Here" << endl;
	soundComp->registerSound("Pull", buffer2);

	gameObject->add_component(soundComp);

#pragma endregion


#pragma region physic

	// rigidbody and collider
	const auto rigid_comp = make_shared<RigidBodyComponent>(*gameObject, b2_dynamicBody);
	//create the collider: 
	b2PolygonShape shape;
	auto go = gameObject;
	auto sprite = go->get_component<AnimationComponent>()->getCurrentAnimation()->getSprite();

	const auto w = (sprite.getLocalBounds().width / 2.) * PhysicsManager::UNRATIO;
	const auto h = (sprite.getLocalBounds().height / 2.) * PhysicsManager::UNRATIO;
	shape.SetAsBox(w * 0.7f, h * 0.7f, b2Vec2(w, h), 0);

	float area = (2 * w * 0.7f) * (2 * h * 0.7f);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 5.f / area;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
	auto colliderComp = make_shared<ColliderComponent>(*gameObject, *rigid_comp, FixtureDef);
	//colliderComp->

	auto handComponent = make_shared<HandMoveComponent>(*gameObject, *rigid_comp, index, index == 0 ? false : true);
	handComponent->setBodyReference(parent);
	//Extend Physics manager and Collider Component to get detailed collision information.
	colliderComp->registerOnCollisionFunction(
		[handComponent](ColliderComponent& collider1, ColliderComponent& collider2)
		{
			handComponent->onCollisionEnter(collider2);
		});

	gameObject->add_component(handComponent);
	gameObject->add_component(rigid_comp);
	gameObject->add_component(colliderComp);

	// joints
	b2RopeJointDef jointDefinition;
	jointDefinition.bodyA = parent->get_component<RigidBodyComponent>()->getB2Body();
	jointDefinition.bodyB = rigid_comp->getB2Body();
	jointDefinition.maxLength = 40.f;
	jointDefinition.localAnchorA = parent->get_component<RigidBodyComponent>()->getB2Body()->GetLocalCenter();
	jointDefinition.localAnchorB = rigid_comp->getB2Body()->GetLocalCenter();

	//b2DistanceJoint* joint = (b2DistanceJoint*)
	auto joint = (b2RopeJoint*)PhysicsManager::get_b2_world()->CreateJoint(&jointDefinition);
	handComponent->setJoint(joint);
#pragma endregion

	gameObject->init();
	return gameObject;
}


GameObject::ptr TileMapManager::makePlayer(Player playerStruct, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(playerStruct.PlayerBody->name, playerStruct.PlayerBody->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(playerStruct.PlayerBody->x), static_cast<float>(playerStruct.PlayerBody->y));

	std::string spriteTexture;
	auto input = false;
	auto player_idx = 0;
	auto mass = 1.0f;
	auto hasArms = false;
	auto speed = 0.f;

	for (auto property : playerStruct.PlayerBody->properties)
	{
		auto name = property->name;
		if (name == "InputPlayerIdx")
		{
			input = true;
			player_idx = stoi(property->value);
		}
		else if (name == "Mass")
		{
			mass = stof(property->value);
		}
		else if (name == "MoveSpeed")
		{
			hasArms = true;
			speed = stof(property->value);
		}
	}

	auto animationComponent = makeAnimationComponent(playerStruct.PlayerBody, gameObject, layer, spriteManager, { "Default", "Jump", "Dead", "Win" }, "../assets/Sprites/", 0.2f, false, true);

#pragma region Physic

	const auto rigid_comp = make_shared<RigidBodyComponent>(*gameObject, b2_dynamicBody);
	//create the collider: 
	b2PolygonShape shape;
	auto go = gameObject;
	auto sprite = go->get_component<AnimationComponent>()->getAnimation("Default")->getSprite();


	const auto w = (sprite.getLocalBounds().width / 2.f) * PhysicsManager::UNRATIO;
	const auto h = (sprite.getLocalBounds().height / 2.f) * PhysicsManager::UNRATIO;
	shape.SetAsBox(w* 0.5f, h * 0.7f, b2Vec2(w, h), 0);

	float area = (2 * w * 0.5f) * (2 * h * 0.7f);

	b2FixtureDef FixtureDef;
	FixtureDef.density = mass / area;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
	auto colliderComp = make_shared<ColliderComponent>(*gameObject, *rigid_comp, FixtureDef);


	gameObject->add_component(rigid_comp);
	gameObject->add_component(colliderComp);

#pragma endregion 

	if (input)
	{
		const auto body_comp =
			make_shared<PlayerBodyComponent>(*gameObject, *rigid_comp);
		gameObject->add_component(body_comp);

		colliderComp->registerOnCollisionFunction(
			[body_comp](ColliderComponent& collider1, ColliderComponent& collider2)
		{
			body_comp->onCollision(collider2);
		});

		if (hasArms)
		{
			auto arm1 = createHand(playerStruct.PlayerHandLeft, layer, gameObject, 0, speed, gameObject->getPosition(), 500.f, spriteManager);
			auto arm2 = createHand(playerStruct.PlayerHandRight, layer, gameObject, 1, speed, gameObject->getPosition(), 500.f, spriteManager);

			// references to other hand
			auto hmc = arm1->get_component<HandMoveComponent>();
			hmc->setOtherHandReference(arm2->get_component<HandMoveComponent>());
			//hmc->setBodyReference(gameObject);
			body_comp->addHand(hmc);

			hmc = arm2->get_component<HandMoveComponent>();
			hmc->setOtherHandReference(arm1->get_component<HandMoveComponent>());
			//hmc->setBodyReference(gameObject);
			body_comp->addHand(hmc);

			// sound
			auto buffer = std::make_shared<sf::SoundBuffer>();
			if (!buffer->loadFromFile("../assets/Sounds/hit.ogg"))
				cout << "No music Here" << endl;

			auto soundComp = std::make_shared<SoundComponent>(*gameObject);
			soundComp->registerSound("Hit", buffer);
			gameObject->add_component(soundComp);

		}
	}

	gameObject->init();
	return gameObject;
}

GameObject::ptr TileMapManager::loadCollider(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name + to_string(object->id), object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	makePhysicsWithObject(object, gameObject, false);

	gameObject->init();

	return gameObject;
}

GameObject::ptr TileMapManager::loadGoalTrigger(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name, object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	auto spriteComponent = makeRenderComponent(object, gameObject, layer, spriteManager);
	//spriteComponent->init();
	makePhysics(gameObject, true);

	//Extend Physics manager and Collider Component to get detailed collision information.
	gameObject->get_component<ColliderComponent>()->registerOnCollisionFunction(
		[](ColliderComponent& collider1, ColliderComponent& collider2)
		{
			if (collider2.getGameObject().getTag() == "Player")
			{
				cout << "Player win" << endl;
				collider2.getGameObject().get_component<AnimationComponent>()->setAnimation("Win");
				collider2.getGameObject().get_component<PlayerBodyComponent>()->playerWon();
			}
		});

	gameObject->init();
	return gameObject;
}

GameObject::ptr TileMapManager::loadTrigger(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name + to_string(object->id), object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	makePhysicsWithObject(object, gameObject, true);

	gameObject->init();

	return gameObject;
}


GameObject::ptr TileMapManager::loadRock(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name + to_string(object->id), object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	for (auto property : object->properties)
	{
		auto name = property->name;

		if (name == "RockType")
		{
			auto value = property->value;
			if (value == "Timer")
			{
				auto rockTimer = std::make_shared<RockTimedComponent>(*gameObject);
				gameObject->add_component(rockTimer);

				auto animation = makeAnimationComponent(object, gameObject, layer, spriteManager, { "Sleep", "Wake", "Angry" }, "../assets/Sprites/", 0.2f, false, true);

			}
			else if (value == "OneTime")
			{
				auto oneTimeRock = std::make_shared<RockOneTimeComponent>(*gameObject);
				gameObject->add_component(oneTimeRock);

				auto buffer = std::make_shared<sf::SoundBuffer>();
				if (!buffer->loadFromFile("../assets/Sounds/skull_ded.ogg"))
					cout << "No music Here" << endl;

				auto soundComp = std::make_shared<SoundComponent>(*gameObject);
				soundComp->registerSound("Dead", buffer);
				gameObject->add_component(soundComp);


				auto renderComonent = makeRenderComponent(object, gameObject, layer, spriteManager);
			}
			else if (value == "Normal")
			{
				auto normalRock = std::make_shared<RockNormalComponent>(*gameObject);
				gameObject->add_component(normalRock);

				auto animatinRenderComponent = makeAnimationComponent(object, gameObject, layer, spriteManager, { "Default", "Grabbed" }, "../assets/Sprites/", 0.2f, false, true);
			}
		}
	}

	makePhysics(gameObject, true, 0.4f, false, true);

	gameObject->init();
	return gameObject;
}

GameObject::ptr TileMapManager::loadLava(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name, object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	auto spriteComponent = makeRenderComponent(object, gameObject, layer, spriteManager);

	makePhysics(gameObject, true, 1.f, false, true);

	//Extend Physics manager and Collider Component to get detailed collision information.
	gameObject->get_component<ColliderComponent>()->registerOnCollisionFunction(
		[](ColliderComponent& collider1, ColliderComponent& collider2)
		{
			if (collider2.getGameObject().getTag() == "Player")
			{
				cout << "Player died" << endl;
				collider2.getGameObject().get_component<AnimationComponent>()->setAnimation("Dead");
				collider2.getGameObject().get_component<PlayerBodyComponent>()->playerDied();
			}
		});

	auto constantVelocity = std::make_shared<ConstantVelocityComponent>(*gameObject, sf::Vector2f(0, -110));
	gameObject->add_component(constantVelocity);

	gameObject->init();

	return gameObject;
}

GameObject::ptr TileMapManager::loadSpitter(NLTmxMapObject* object, const std::string& layer,
                                            const std::string& resourcePath,
                                            SpriteManager& spriteManager)
{
		std::shared_ptr<GameObject> gameObject;
		gameObject = make_shared<GameObject>(object->name + to_string(object->id), "Spitter");
		EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));
		gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

		std::string slimeTexturePath;
		bool isKinematic = true;
		bool isMirrored = false;
		int triggerIndex = 0;

		for (auto property : object->properties)
		{
			auto name = property->name;
			if (name == "SlimeTexture")
			{
				slimeTexturePath = resourcePath + "Sprites/" + property->value;
			}
			else if (name == "Kinematic")
			{
				property->value == "true" ? isKinematic = true : isKinematic = false;
			}
			else if (name == "Mirrored")
			{
				property->value == "true" ? isMirrored = true : isMirrored = false;
			}
			else if (name == "TriggerIndex")
			{
				triggerIndex = stoi(property->value);
			}
		}


#pragma region components
		auto animationComponent = makeAnimationComponent(object, gameObject, layer, spriteManager, { "Default", "Spitting" }, "../assets/Sprites/", 0.5f, false, false);

		auto textureRect = animationComponent->getCurrentAnimation()->getSprite().getTextureRect();

		// in which direction he lookin?
		if (isMirrored)
		{
			gameObject->setOrigin(textureRect.width, 0);
			gameObject->setScale(-1, 1);
		}
	
		// sound
		auto buffer = std::make_shared<sf::SoundBuffer>();
		if (!buffer->loadFromFile("../assets/Sounds/shoot.ogg"))
			cout << "No music Here" << endl;

		auto soundComp = std::make_shared<SoundComponent>(*gameObject);
		soundComp->registerSound("Shoot", buffer);
		gameObject->add_component(soundComp);

		// functionality
		auto spitterComp = std::make_shared<SpitterComponent>(*gameObject);
		spitterComp->setMirrored(isMirrored);
		spitterComp->setSpitterIndex(triggerIndex);
		gameObject->add_component(spitterComp);
#pragma endregion

#pragma region slimes

		for (int i = 0; i < 10; i++)
		{
			auto slime = make_shared<GameObject>("Slime" + gameObject->getId() + to_string(i), "Slime");
			EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(slime));

			slime->move(-100000, -100000);

			auto renderComponent =
				std::make_shared<SpriteRenderComponent>(*slime, spriteManager.getWindow(), slimeTexturePath);

			renderComponent->init();

			EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>("BehindObjects", *renderComponent));

			slime->add_component(renderComponent);

			makePhysics(slime, isKinematic);

			slime->init();

			spitterComp->addSlime(slime);
		}

		gameObject->init();

#pragma endregion
		return gameObject;
}

GameObject::ptr TileMapManager::loadSpitterTrigger(NLTmxMapObject* object, const std::string& layer,
                                                   const std::string& resourcePath,
                                                   SpriteManager& spriteManager)
{
	int triggerIndex = 0;
	for (auto p : object->properties)
	{
		if (p->name == "TriggerIndex")
			triggerIndex = stoi(p->value);
	}

	auto spitterTrigger = make_shared<GameObject>(object->name + std::to_string(object->id), object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(spitterTrigger));

	spitterTrigger->move(static_cast<float>(object->x), static_cast<float>(object->y));

	makePhysicsWithObject(object, spitterTrigger, true);

	spitterTrigger->init();
	return spitterTrigger;
}

void TileMapManager::setupSpitter(const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	std::vector<GameObject::ptr> spitter;

	for (auto s : mSpitterBody)
	{
		spitter.push_back(loadSpitter(s, layer, resourcePath, spriteManager));
	}
	
	for each (auto triggerObj in mSpitterTrigger)
	{
		int triggerIndex = 0;
		for (auto p : triggerObj->properties)
		{
			if (p->name == "TriggerIndex")
				triggerIndex = stoi(p->value);
		}

		auto trigger = loadSpitterTrigger(triggerObj, layer, resourcePath, spriteManager);


		for (auto s : spitter)
		{
			if (s->get_component<SpitterComponent>()->getSpitterIndex() == triggerIndex)
			{
				trigger->get_component<ColliderComponent>()->registerOnCollisionFunction(
					[s](ColliderComponent& collider1, ColliderComponent& collider2)
					{
						s->get_component<SpitterComponent>()->startSpitting();
					});
			}
		}
	}

	mSpitterBody.clear();
	mSpitterTrigger.clear();
}

GameObject::ptr TileMapManager::loadButton(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager, GuiManager* guiManager)
{
	auto gameObject = make_shared<GameObject>(object->name, object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	bool f = false;

	for (auto property : object->properties)
	{
		auto name = property->name;
		if (name == "Focused")
		{
			f = property->value == "true" ? true : false;
		}
	}

	auto animationComp = makeAnimationComponent(object, gameObject, layer, spriteManager, { "Default", "Focused" }, "../assets/", 0.2, false, false);

	f == false ? animationComp->setAnimation("Default") : animationComp->setAnimation("Focused");

	auto soundComp = std::make_shared<SoundComponent>(*gameObject);

	auto buffer2 = std::make_shared<sf::SoundBuffer>();
	if (!buffer2->loadFromFile("../assets/Sounds/grab.ogg"))
		cout << "No music Here" << endl;
	soundComp->registerSound("Change", buffer2);

	gameObject->add_component(soundComp);
	
	guiManager->addButton(gameObject, f);
	
	gameObject->init();
	return gameObject;
}

GameObject::ptr TileMapManager::loadDeadOverlay(SpriteManager& spriteManager)
{
	auto gameObject = std::make_shared<GameObject>("Dead", "Die");
	gameObject->move(-100000, -100000);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	auto r =
		std::make_shared<SpriteRenderComponent>(*gameObject, spriteManager.getWindow(), "../assets/Sprites/die.png");
	r->init();
	EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>("Top", *r));
	gameObject->add_component(r);

	return gameObject;
}

GameObject::ptr TileMapManager::loadWinOverlay(SpriteManager& spriteManager)
{
	auto gameObject = std::make_shared<GameObject>("Win", "Win");
	gameObject->move(-100000, -100000);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	auto r =
		std::make_shared<SpriteRenderComponent>(*gameObject, spriteManager.getWindow(), "../assets/Sprites/youwin.png");
	r->init();
	EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>("Top", *r));
	gameObject->add_component(r);

	return gameObject;
}



void TileMapManager::makePhysics(GameObject::ptr gameObject, bool isKinematic, float sizeFactor, bool isStatic, bool isSensor, float density, float friction)
{
	const auto rigid_comp = make_shared<RigidBodyComponent>(*gameObject, b2_staticBody);
	//create the collider: 
	b2PolygonShape shape;
	auto go = gameObject;
	auto sprite = go->get_component<SpriteRenderComponent>();
	auto animation = go->get_component<AnimationComponent>();

	Sprite s;

	if (!sprite)
		s = animation->getCurrentAnimation()->getSprite();
	else
		s = sprite->getSprite();

	const auto w = ((s.getLocalBounds().width) / 2.) * PhysicsManager::UNRATIO;
	const auto h = ((s.getLocalBounds().height) / 2.) * PhysicsManager::UNRATIO;

	const auto sw = w * sizeFactor;
	const auto sh = h * sizeFactor;
	shape.SetAsBox(sw, sh, b2Vec2(w, h), 0);

	if (isKinematic)
		rigid_comp->getB2Body()->SetType(b2_kinematicBody);
	else if (isStatic)
		rigid_comp->getB2Body()->SetType(b2_staticBody);
	else
		rigid_comp->getB2Body()->SetType(b2_dynamicBody);



	b2FixtureDef FixtureDef;
	FixtureDef.density = density;
	FixtureDef.friction = friction;
	FixtureDef.shape = &shape;
	FixtureDef.isSensor = isSensor;
	auto colliderComp = make_shared<ColliderComponent>(*gameObject, *rigid_comp, FixtureDef);

	gameObject->add_component(rigid_comp);
	gameObject->add_component(colliderComp);
}

void TileMapManager::makePhysicsWithObject(NLTmxMapObject* object, GameObject::ptr gameObject, bool isSensor)
{
	// Parse data from file
	IntRect texture_rect{};
	texture_rect.width = object->width;
	texture_rect.height = object->height;

	const auto rigid_comp = make_shared<RigidBodyComponent>(*gameObject, b2_staticBody);
	//create the collider: 
	b2PolygonShape shape;

	auto w = (texture_rect.width / 2.) * PhysicsManager::UNRATIO;
	auto h = (texture_rect.height / 2.) * PhysicsManager::UNRATIO;
	double cosRotation = cos(degreesToRadians(object->rotation));
	double sinRotation = sin(degreesToRadians(object->rotation));
	double rotatedCenterX = w * cosRotation - h * sinRotation;
	double rotatedCenterY = w * sinRotation + h * cosRotation;

	
	shape.SetAsBox(w, h, b2Vec2(rotatedCenterX, rotatedCenterY), degreesToRadians(object->rotation));

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
	FixtureDef.isSensor = isSensor;

	auto colliderComp = make_shared<ColliderComponent>(*gameObject, *rigid_comp, FixtureDef);

	gameObject->add_component(rigid_comp);
	gameObject->add_component(colliderComp);
}

std::shared_ptr<SpriteRenderComponent> TileMapManager::makeRenderComponent(NLTmxMapObject* object, std::shared_ptr<GameObject> gameObject, string layer, SpriteManager& spriteManager, string texturePathOptional)
{
	std::shared_ptr<SpriteRenderComponent> renderComponent;

	sf::IntRect textureRect{};
	textureRect.width = object->width;
	textureRect.height = object->height;

	for (auto property : object->properties)
	{
		auto name = property->name;

		if (name == "Texture")
		{
			string path;
			texturePathOptional.empty() ? path = "../assets/Sprites/" + property->value : path = texturePathOptional;

			renderComponent =
				std::make_shared<SpriteRenderComponent>(*gameObject, spriteManager.getWindow(), path);
		}
		else if (name == "TextureRectLeft")
		{
			textureRect.left = stoi(property->value);
		}
		else if (name == "TextureRectTop")
		{
			textureRect.top = stoi(property->value);
		}
	}
	gameObject->add_component(renderComponent);


	renderComponent->getSprite().setTextureRect(textureRect);
	//renderComp->getSprite().setOrigin(textureRect.width * 0.5f, textureRect.height *0.5f);
	//renderComp->getSprite().setPosition(0.0f, 0.0f);

	EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *renderComponent));

	return renderComponent;
}

std::shared_ptr<AnimationComponent> TileMapManager::makeAnimationComponent(NLTmxMapObject* object, std::shared_ptr<GameObject> gameObject, string layer, SpriteManager& spriteManager, std::vector<std::string> animationNames, string ressourcePath, float frameTime, bool paused, bool looped,  string texturePathOptional)
{
	std::shared_ptr<AnimationComponent> animationComponent = std::make_shared<AnimationComponent>(*gameObject, spriteManager.getWindow(), frameTime, paused, looped);

	sf::IntRect textureRect{};
	textureRect.width = object->width;
	textureRect.height = object->height;
	string path;


	for (auto property : object->properties)
	{
		auto name = property->name;

		if (name == "Texture")
		{
			texturePathOptional.empty() ? path = ressourcePath + property->value : path = texturePathOptional;
		}
		else if (name == "TextureRectLeft")
		{
			textureRect.left = stoi(property->value);
		}
		else if (name == "TextureRectTop")
		{
			textureRect.top = stoi(property->value);
		}
	}

	for (int i = 0; i < animationNames.size(); i++)
	{
		auto animation = makeAnimation(0, textureRect.height * i, textureRect.width, textureRect.height, path);
		animationComponent->registerAnimation(animationNames[i], animation);
	}
	animationComponent->setAnimation(animationNames[0]);
	gameObject->add_component(animationComponent);
	EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *animationComponent));
	return animationComponent;
}

Animation::ptr TileMapManager::makeAnimation(int rectLeft, int rectTop, int frameWidth, int frameHeight, std::string path)
{
	auto animation = std::make_shared<Animation>(path);

	while (rectLeft < animation->getSprite().getTexture()->getSize().x)
	{
		animation->addFrame(sf::IntRect(rectLeft, rectTop, frameWidth, frameHeight));
		rectLeft += frameWidth;
	}

	return animation;
}