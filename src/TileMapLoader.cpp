#include "stdafx.h"

#include "TileMapLoader.h"
#include "NLTmxMap.h"
#include "TileLayerRenderComponent.h"
#include "SpriteRenderComponent.h"
#include "PlayerMoveComponent.h"
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
#include "SpitterTriggerComponent.h"
#include "ConstantVelocityComponent.h"


struct Player
{
	NLTmxMapObject* PlayerBody;
	NLTmxMapObject* PlayerHandLeft;
	NLTmxMapObject* PlayerHandRight;
};

struct Spitter
{
	NLTmxMapObject* SpitterBody;
	NLTmxMapObject* SpitterTrigger;
};

void loadTileLayers(NLTmxMap* tilemap, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	err() << "Load tilemap with size: " << tilemap->width << ", "
		<< tilemap->height << " and tile-size: " << tilemap->tileWidth
		<< ", " << tilemap->tileHeight << std::endl;

	spriteManager.setTileSize({ tilemap->tileWidth, tilemap->tileHeight });

	// load textures for every tileset
	SpriteManager::TileSetMap tileSets;
	for (auto tileset : tilemap->tilesets)
	{
		err() << "Load tileset: " << tileset->name << " width filename: "
			<< tileset->filename << " and tile-size: " << tileset->tileWidth
			<< ", " << tileset->tileHeight << std::endl;

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
		err() << "Load layer: " << layer->name << " with width: "
			<< layer->width << " and height: " << layer->height << std::endl;

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




static Animation::ptr MakeAnimation(int rectLeft, int rectTop, int frameWidth, std::string path)
{
	auto animation = std::make_shared<Animation>(path);

	while (rectLeft < animation->getSprite().getTexture()->getSize().x)
	{
		animation->addFrame(sf::IntRect(rectLeft, rectTop, frameWidth, frameWidth));
		rectLeft += frameWidth;
	}

	return animation;
}


static std::shared_ptr<SpriteRenderComponent> makeRenderComponent(NLTmxMapObject* object, std::shared_ptr<GameObject> gameObject, string layer, SpriteManager& spriteManager, string texturePathOptional = "")
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
			texturePathOptional.empty() ? path = "../assets/sprites/" + property->value : path = texturePathOptional;

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


static GameObject::ptr createHand(NLTmxMapObject* object, const std::string& layer, b2Body& parent, const int index, const float speed, const Vector2f startPos, float distanceFromStart, SpriteManager& spriteManager)
{
	std::string name = "Hand" + to_string(index);
	auto gameObject = make_shared<GameObject>(name, "Hand");
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(startPos.x + (index == 0 ? -distanceFromStart : distanceFromStart)), static_cast<float>(startPos.y - distanceFromStart / 2));

	string path;
	sf::IntRect textureRect{};
	textureRect.width = object->width;
	textureRect.height = object->height;
	for (auto property : object->properties)
	{
		auto name = property->name;

		if (name == "Texture")
		{
			path = "../assets/sprites/" + property->value;

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

#pragma region animation

	auto closedToOPen = MakeAnimation(0, 0, textureRect.height, path);
	auto openToClosed = MakeAnimation(0, textureRect.height , textureRect.height, path);

	//closedToOPen->getSprite().setScale(1.5f, 1.5f);
	//openToClosed->getSprite().setScale(1.5f, 1.5f);

	auto animatedSprite = make_shared<AnimationComponent>(*gameObject, spriteManager.getWindow(), 0.2f, false, true);
	animatedSprite->registerAnimation("ToOpen" + to_string(index), closedToOPen);
	animatedSprite->registerAnimation("ToClose" + to_string(index), openToClosed);
	animatedSprite->setLooped(false);

	animatedSprite->setAnimation("ToOpen" + to_string(index));
	EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *animatedSprite));

	gameObject->add_component(animatedSprite);

#pragma endregion

#pragma region Sound

	auto buffer = std::make_shared<sf::SoundBuffer>();
	if (!buffer->loadFromFile("../assets/Sounds/chicken.wav"))
		cout << "No music Here" << endl;

	auto soundComp = std::make_shared<SoundComponent>(*gameObject);
	soundComp->registerSound("Grab", buffer);

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

	auto handComponent = make_shared<HandMoveComponent>(*gameObject, *rigid_comp, index, index == 0 ? false : true);

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
	jointDefinition.bodyA = &parent;
	jointDefinition.bodyB = rigid_comp->getB2Body();
	jointDefinition.maxLength = 40.f;
	jointDefinition.localAnchorA = *&parent.GetLocalCenter();
	jointDefinition.localAnchorB = rigid_comp->getB2Body()->GetLocalCenter();

	//b2DistanceJoint* joint = (b2DistanceJoint*)
	auto joint = (b2RopeJoint*)PhysicsManager::get_b2_world()->CreateJoint(&jointDefinition);
	handComponent->setJoint(joint);
#pragma endregion

	gameObject->init();
	return gameObject;
}

static void makePhysics(GameObject::ptr gameObject, bool isKinematic, float sizeFactor = 1.0f)
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
	else
		rigid_comp->getB2Body()->SetType(b2_dynamicBody);



	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.0f;
	FixtureDef.shape = &shape;
	FixtureDef.isSensor = true;
	auto colliderComp = make_shared<ColliderComponent>(*gameObject, *rigid_comp, FixtureDef);

	gameObject->add_component(rigid_comp);
	gameObject->add_component(colliderComp);
}

static GameObject::ptr makePlayer(Player playerStruct, const std::string& layer, const std::string& resourcePath,
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
	string path;
	sf::IntRect textureRect{};
	textureRect.width = playerStruct.PlayerBody->width;
	textureRect.height = playerStruct.PlayerBody->height;

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
		else if (name == "Texture")
		{
			path = "../assets/sprites/" + property->value;

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

#pragma region Animation


	auto defaultFace = MakeAnimation(0, 0, 768, path);
	auto dead = MakeAnimation(0, 768 * 2, 768, path);
	auto jump = MakeAnimation(0, 768, 768, path);
	auto win = MakeAnimation(0, 768 * 3, 768, path);


	auto animatedSprite = make_shared<AnimationComponent>(*gameObject, spriteManager.getWindow(), 0.2f, false, true);
	animatedSprite->registerAnimation("Default", defaultFace);
	animatedSprite->registerAnimation("Dead", dead);
	animatedSprite->registerAnimation("Jump", jump);
	animatedSprite->registerAnimation("Win", win);


	animatedSprite->setAnimation("Default");
	EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *animatedSprite));

	gameObject->add_component(animatedSprite);

#pragma endregion 

#pragma region Physic

	const auto rigid_comp = make_shared<RigidBodyComponent>(*gameObject, b2_dynamicBody);
	//create the collider: 
	b2PolygonShape shape;
	auto go = gameObject;
	auto sprite = go->get_component<AnimationComponent>()->getAnimation("Default")->getSprite();


	const auto w = (sprite.getLocalBounds().width / 2.) * PhysicsManager::UNRATIO;
	const auto h = (sprite.getLocalBounds().height / 2.) * PhysicsManager::UNRATIO;
	shape.SetAsBox(w* 0.5f, h * 0.7f, b2Vec2(w, h), 0);

	float area = (2 * w * 0.5f) * (2 * h * 0.7f);

	b2FixtureDef FixtureDef;
	FixtureDef.density = mass / area;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
	auto colliderComp = make_shared<ColliderComponent>(*gameObject, *rigid_comp, FixtureDef);

	//Extend Physics manager and Collider Component to get detailed collision information.
	//colliderComp->registerOnCollisionFunction(
	//	[](ColliderComponent& collider1, ColliderComponent& collider2)
	//	{
	//		cout << "Collision: " << collider1.getGameObject().getId() << " vs. " << collider2
	//		                                                                            .getGameObject().getId() <<
	//			endl;
	//	});

	gameObject->add_component(rigid_comp);
	gameObject->add_component(colliderComp);

#pragma endregion 

	if (input)
	{
		const auto body_comp =
			make_shared<PlayerBodyComponent>(*gameObject, *rigid_comp);
		gameObject->add_component(body_comp);
	}

	if (hasArms)
	{
		auto arm1 = createHand(playerStruct.PlayerHandLeft, layer, *rigid_comp->getB2Body(), 0, speed, gameObject->getPosition(), 500.f,  spriteManager);
		auto arm2 = createHand(playerStruct.PlayerHandRight, layer, *rigid_comp->getB2Body(), 1, speed, gameObject->getPosition(), 500.f,  spriteManager);

		// references to other hand
		auto hmc = arm1->get_component<HandMoveComponent>();
		hmc->setOtherHandReference(arm2->get_component<HandMoveComponent>());
		hmc->setBodyReference(gameObject);

		hmc = arm2->get_component<HandMoveComponent>();
		hmc->setOtherHandReference(arm1->get_component<HandMoveComponent>());
		hmc->setBodyReference(gameObject);
	}

	gameObject->init();
	return gameObject;
}

static GameObject::ptr loadCollider(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name + to_string(object->id), object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	// Parse data from file
	IntRect texture_rect{};
	texture_rect.width = object->width;
	texture_rect.height = object->height;

	const auto rigid_comp = make_shared<RigidBodyComponent>(*gameObject, b2_staticBody);
	//create the collider: 
	b2PolygonShape shape;
	auto go = gameObject;

	const auto w = (texture_rect.width / 2.) * PhysicsManager::UNRATIO;
	const auto h = (texture_rect.height / 2.) * PhysicsManager::UNRATIO;
	shape.SetAsBox(w, h, b2Vec2(w, h), 0);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
	auto colliderComp = make_shared<ColliderComponent>(*gameObject, *rigid_comp, FixtureDef);

	//Extend Physics manager and Collider Component to get detailed collision information.
	/*colliderComp->registerOnCollisionFunction(
		[](ColliderComponent& collider1, ColliderComponent& collider2)
	{
		cout << "Collision: " << collider1.getGameObject().getId() << " vs. " << collider2
			.getGameObject().getId() <<
			endl;
	});*/

	gameObject->add_component(rigid_comp);
	gameObject->add_component(colliderComp);



	gameObject->init();

	return gameObject;
}

static GameObject::ptr loadTrigger(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name + to_string(object->id), object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	// Parse data from file
	IntRect texture_rect{};
	texture_rect.width = object->width;
	texture_rect.height = object->height;

	const auto rigid_comp = make_shared<RigidBodyComponent>(*gameObject, b2_staticBody);
	//create the collider: 
	b2PolygonShape shape;
	auto go = gameObject;

	const auto w = (texture_rect.width / 2.) * PhysicsManager::UNRATIO;
	const auto h = (texture_rect.height / 2.) * PhysicsManager::UNRATIO;
	shape.SetAsBox(w, h, b2Vec2(w, h), 0);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
	FixtureDef.isSensor = true;
	auto colliderComp = make_shared<ColliderComponent>(*gameObject, *rigid_comp, FixtureDef);

	//Extend Physics manager and Collider Component to get detailed collision information.
	colliderComp->registerOnCollisionFunction(
		[](ColliderComponent& collider1, ColliderComponent& collider2)
		{
			cout << "Collision: " << collider1.getGameObject().getId() << " vs. " << collider2
				.getGameObject().getId() <<
				endl;
		});

	gameObject->add_component(rigid_comp);
	gameObject->add_component(colliderComp);



	gameObject->init();

	return gameObject;
}


static GameObject::ptr loadRock(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name + to_string(object->id), object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	// Parse data from file
	IntRect texture_rect{};
	texture_rect.width = object->width;
	texture_rect.height = object->height;
	std::string spriteTexture;

	for (auto property : object->properties)
	{
		auto name = property->name;
		if (name == "Texture")
		{
			spriteTexture = resourcePath + "Sprites/" + property->value;
		}
		else if (name == "TextureSize")
		{
			//int rectLeftStart = rand() % 3;
			//int rectTopStart = rand() % 3;

			texture_rect.left = stoi(property->value);
			texture_rect.top = stoi(property->value);

		}

	}

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

				auto sleep = MakeAnimation(0, 0, texture_rect.width, spriteTexture);
				auto wake = MakeAnimation(0, texture_rect.height, texture_rect.width, spriteTexture);
				auto angry = MakeAnimation(0, texture_rect.height * 2, texture_rect.width, spriteTexture);
				//sleep->getSprite().setScale(1.5f, 1.5f);

				//wake->getSprite().setScale(1.5f, 1.5f);
				//angry->getSprite().setScale(1.5f, 1.5f);


				auto animationComp = std::make_shared<AnimationComponent>(*gameObject, spriteManager.getWindow(), 0.2f, false, true);
				animationComp->registerAnimation("Sleep", sleep);
				animationComp->registerAnimation("Wake", wake);
				animationComp->registerAnimation("Angry", angry);

				animationComp->setAnimation("Sleep");

				EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *animationComp));
				gameObject->add_component(animationComp);
			}
			else if (value == "OneTime")
			{
				auto oneTimeRock = std::make_shared<RockOneTimeComponent>(*gameObject);
				gameObject->add_component(oneTimeRock);


				// Initialize components with parsed data.
				if (spriteTexture.length() > 0)
				{
					auto render_comp = make_shared<SpriteRenderComponent>(
						*gameObject, spriteManager.getWindow(), spriteTexture
						);
					gameObject->add_component(render_comp);

					render_comp->getSprite().setTextureRect(texture_rect);
					//render_comp->getSprite().setScale(1.5f, 1.5f);
					//renderComp->getSprite().setOrigin(textureRect.width * 0.5f, textureRect.height *0.5f);
					//renderComp->getSprite().setPosition(0.0f, 0.0f);

					EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *render_comp));
				}
			}
			else if (value == "Normal")
			{
				auto normalRock = std::make_shared<RockNormalComponent>(*gameObject);
				gameObject->add_component(normalRock);

				auto defaut = MakeAnimation(0, 0, texture_rect.width, spriteTexture);
				auto Gabbed = MakeAnimation(0, texture_rect.height, texture_rect.width, spriteTexture);

				//defaut->getSprite().setScale(1.5f, 1.5f);
				//Gabbed->getSprite().setScale(1.5f, 1.5f);


				auto animationComp = std::make_shared<AnimationComponent>(*gameObject, spriteManager.getWindow(), 0.2f, false, true);
				animationComp->registerAnimation("Default", defaut);
				animationComp->registerAnimation("Grabbed", Gabbed);

				animationComp->setAnimation("Default");

				EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *animationComp));
				gameObject->add_component(animationComp);
			}
		}
	}

	makePhysics(gameObject, true, 0.7f);

	gameObject->init();
	return gameObject;
}

static GameObject::ptr loadLava(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name, object->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	auto spriteComponent = makeRenderComponent(object, gameObject, layer, spriteManager);

	makePhysics(gameObject, true);


	//Extend Physics manager and Collider Component to get detailed collision information.
	gameObject->get_component<ColliderComponent>()->registerOnCollisionFunction(
		[](ColliderComponent& collider1, ColliderComponent& collider2)
		{
			cout << "Collision: " << collider1.getGameObject().getId() << " vs. " << collider2
				.getGameObject().getId() <<
				endl;

			if (collider2.getGameObject().getTag() == "Player")
			{
				cout << "Player died" << endl;
				collider2.getGameObject().get_component<AnimationComponent>()->setAnimation("Dead");
			}


		});

	auto constantVelocity = std::make_shared<ConstantVelocityComponent>(*gameObject, sf::Vector2f(0, -200));
	gameObject->add_component(constantVelocity);

	gameObject->init();
	//gameObject->get_component<RigidBodyComponent>()->getB2Body()->SetSleepingAllowed(false);
	//gameObject->get_component<RigidBodyComponent>()->getB2Body()->SetLinearVelocity(b2Vec2(0, -10000000));
	//gameObject->get_component<RigidBodyComponent>()->getB2Body()->ApplyLinearImpulse(b2Vec2(0, -100000), b2Vec2(0, 0), true);
	return gameObject;
}

static GameObject::ptr loadSpitter(Spitter spitter, const std::string& layer, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(spitter.SpitterBody->name + to_string(spitter.SpitterBody->id), "Hand");
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(spitter.SpitterBody->x), static_cast<float>(spitter.SpitterBody->y));

	sf::IntRect textureRect{};
	textureRect.width = spitter.SpitterBody->width;
	textureRect.height = spitter.SpitterBody->height;
	std::string texturePath;
	std::string slimeTexturePath;
	bool isKinematic;

	for (auto property : spitter.SpitterBody->properties)
	{
		auto name = property->name;

		if (name == "Texture")
		{
			texturePath = resourcePath + "Sprites/" + property->value;
		}
		else if (name == "SlimeTexture")
		{
			slimeTexturePath = resourcePath + "Sprites/" + property->value;
		}
		else if (name == "TextureRectLeft")
		{
			textureRect.left = stoi(property->value);
		}
		else if (name == "TextureRectTop")
		{
			textureRect.top = stoi(property->value);
		}
		else if (name == "SpitterType")
		{
			property->value == "true" ? isKinematic = true : isKinematic = false;
		}
	}

#pragma region animation

	auto d = MakeAnimation(0, 0, 384, texturePath);
	auto s = MakeAnimation(0, 384, 384, texturePath);


	auto animation = std::make_shared<AnimationComponent>(*gameObject, spriteManager.getWindow());
	animation->registerAnimation("Default", d);
	animation->registerAnimation("Spitting", s);

	EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *animation));
	gameObject->add_component(animation);

#pragma endregion


	auto spitterComp = std::make_shared<SpitterComponent>(*gameObject);
	gameObject->add_component(spitterComp);

#pragma region slimes

	for (int i = 0; i < 20; i++)
	{
		auto slime = make_shared<GameObject>("Slime" + to_string(i), "Slime");
		EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(slime));

		slime->move(static_cast<float>(spitter.SpitterBody->x), static_cast<float>(spitter.SpitterBody->y));

		auto renderComponent =
			std::make_shared<SpriteRenderComponent>(*slime, spriteManager.getWindow(), slimeTexturePath);


		//renderComponent->getSprite().setTextureRect(sf::IntRect(0, 0, ));

		EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>("BehindObjects", *renderComponent));

		slime->add_component(renderComponent);

		makePhysics(slime, true);

		slime->init();

		spitterComp->addSlime(slime);
	}

#pragma endregion

#pragma region SpitterTrigger

	sf::IntRect texture_rect{};
	texture_rect.width = spitter.SpitterTrigger->width;
	texture_rect.height = spitter.SpitterTrigger->height;

	auto spitterTrigger = make_shared<GameObject>(spitter.SpitterTrigger->name, spitter.SpitterTrigger->type);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(spitterTrigger));

	spitterTrigger->move(static_cast<float>(spitter.SpitterTrigger->x), static_cast<float>(spitter.SpitterTrigger->y));


	const auto rigid_comp = make_shared<RigidBodyComponent>(*spitterTrigger, b2_staticBody);
	//create the collider: 
	b2PolygonShape shape;
	auto go = gameObject;

	const auto w = (texture_rect.width / 2.) * PhysicsManager::UNRATIO;
	const auto h = (texture_rect.height / 2.) * PhysicsManager::UNRATIO;
	shape.SetAsBox(w, h, b2Vec2(w, h), 0);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
	FixtureDef.isSensor = true;
	auto colliderComp = make_shared<ColliderComponent>(*spitterTrigger, *rigid_comp, FixtureDef);

	//Extend Physics manager and Collider Component to get detailed collision information.
	colliderComp->registerOnCollisionFunction(
		[](ColliderComponent& collider1, ColliderComponent& collider2)
		{
			cout << "Collision: " << collider1.getGameObject().getId() << " vs. " << collider2
				.getGameObject().getId() <<
				endl;
		});

	spitterTrigger->add_component(rigid_comp);
	spitterTrigger->add_component(colliderComp);

	auto stc = std::make_shared<SpitterTriggerComponent>(spitterTrigger);

	//Extend Physics manager and Collider Component to get detailed collision information.
	spitterTrigger->get_component<ColliderComponent>()->registerOnCollisionFunction(
		[](ColliderComponent& collider1, ColliderComponent& collider2)
		{
			cout << "Collision: " << collider1.getGameObject().getId() << " vs. " << collider2
				.getGameObject().getId() <<
				endl;

			if (collider2.getGameObject().getTag() == "Player")
			{
				//collider1.getGameObject().get_component<SpitterTriggerComponent>()->getSpitter().get_component<SpitterComponent>()->startSpitting();
			}


		});

	spitterTrigger->init();
#pragma endregion

	gameObject->init();
	return gameObject;
}


void loadObjectLayers(NLTmxMap* tilemap, const std::string& resource_path, SpriteManager& sprite_manager)
{
	Player player{};
	Spitter spitter{};

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
			if (object->type == "Player")
				player.PlayerBody = object;
			if (object->type == "PlayerHandLeft")
				player.PlayerHandLeft = object;
			if (object->type == "PlayerHandRight")
				player.PlayerHandRight = object;
			if (object->type == "Spitter")
				spitter.SpitterBody = object;
			if (object->type == "SpitterTrigger")
				spitter.SpitterTrigger = object;
		}
		if (group->name == "GameObjects")
		{
			makePlayer(player, group->name, resource_path, sprite_manager);
			loadSpitter(spitter, group->name, resource_path, sprite_manager);
		}
	}


}
