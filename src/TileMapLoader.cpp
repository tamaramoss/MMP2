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

void loadTileLayers(NLTmxMap* tilemap, const std::string& resourcePath,
                      SpriteManager& spriteManager)
{
	err() << "Load tilemap with size: " << tilemap->width << ", "
		<< tilemap->height << " and tile-size: " << tilemap->tileWidth
		<< ", " << tilemap->tileHeight << std::endl;

	spriteManager.setTileSize({tilemap->tileWidth, tilemap->tileHeight});

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

			tile_layers[layer_idx].tiles.push_back({i, sprite});
		}
	}

	// Add objects to layers
	auto go = make_shared<GameObject>("TileMap");
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

static GameObject::ptr loadSprite(NLTmxMapObject* object, const std::string& layer, const std::string& resourcePath,
                                   SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name);
	EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	// Parse data from file
	IntRect texture_rect{};
	texture_rect.width = object->width;
	texture_rect.height = object->height;
	std::string spriteTexture;
	auto input = false;
	auto player_idx = 0;
	auto mass = 1.0f;
	for (auto property : object->properties)
	{
		auto name = property->name;
		if (name == "Texture")
		{
			spriteTexture = resourcePath + property->value;
		}
		else if (name == "TextureRectLeft")
		{
			texture_rect.left = stoi(property->value);
		}
		else if (name == "TextureRectTop")
		{
			texture_rect.top = stoi(property->value);
		}
		else if (name == "InputPlayerIdx")
		{
			input = true;
			player_idx = stoi(property->value);
		}
		else if (name == "Mass")
		{
			mass = stof(property->value);
		}
	}

	// Initialize components with parsed data.
	if (spriteTexture.length() > 0)
	{
		auto render_comp = make_shared<SpriteRenderComponent>(
			*gameObject, spriteManager.getWindow(), spriteTexture
		);
		gameObject->add_component(render_comp);

		render_comp->getSprite().setTextureRect(texture_rect);
		//renderComp->getSprite().setOrigin(textureRect.width * 0.5f, textureRect.height *0.5f);
		//renderComp->getSprite().setPosition(0.0f, 0.0f);

		EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(layer, *render_comp));
	}

	const auto rigid_comp = make_shared<RigidBodyComponent>(*gameObject, b2_dynamicBody);
	//create the collider: 
	b2PolygonShape shape;
	auto go = gameObject;
	auto sprite = go->get_component<SpriteRenderComponent>();

	const auto w = (sprite->getSprite().getLocalBounds().width / 2.) * PhysicsManager::UNRATIO;
	const auto h = (sprite->getSprite().getLocalBounds().height / 2.) * PhysicsManager::UNRATIO;
	shape.SetAsBox(w, h, b2Vec2(w, h), 0);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
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


	if (input)
	{
		const auto input_comp =
			make_shared<PlayerMoveComponent>(*gameObject, *rigid_comp, player_idx);
		gameObject->add_component(input_comp);
	}

	gameObject->init();
	return gameObject;
}


void loadObjectLayers(NLTmxMap* tilemap, const std::string& resource_path, SpriteManager& sprite_manager)
{
	// go through all object layers
	for (auto group : tilemap->groups)
	{
		// go over all objects per layer
		for (auto object : group->objects)
		{
			const Vector2f position(static_cast<float>(object->x), static_cast<float>(object->y));

			FloatRect bounds(position.x, position.y, static_cast<float>(object->width),
			                 static_cast<float>(object->height));

			if (object->type == "Sprite")
				auto sprite = loadSprite(object, group->name, resource_path, sprite_manager);
		}
	}
}
