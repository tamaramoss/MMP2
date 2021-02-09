#include "StdAfx.h"
#include <cassert>
#include <iostream>

#include "rapidxml/rapidxml.hpp"
#include "NLTmxMap.h"


using namespace rapidxml;

NLTmxMapTileset* NLTmxMap::get_tileset_for_grid(const int grid)
{
	NLTmxMapTileset* result = nullptr;

	for (auto tileset : tilesets)
	{
		if (tileset->firstGid > grid)
		{
			break;
		}

		result = tileset;
	}

	return result;
}

NLTmxMap* NLLoadTmxMap(const std::string& filename)
{
	FileInputStream map_stream;
	if (!map_stream.open(filename))
	{
		err() << "loadMap: could not open file " << filename << endl;
		return nullptr;
	}

	// convert FileInputStream to char* mapBuffer
	const auto size = static_cast<int>(map_stream.getSize()) + 1;
	const auto map_buffer = new char[size];
	map_stream.read(map_buffer, map_stream.getSize());
	map_buffer[map_stream.getSize()] = '\0';

	// now lets load a NLTmxMap
	const auto tilemap = NLLoadTmxMap(map_buffer);
	delete[] map_buffer;

	return tilemap;
}

NLTmxMap* NLLoadTmxMap(char* xml)
{
	xml_document<> doc;
	doc.parse<0>(xml);

	const auto mapnode = doc.first_node("map");

	auto map = new NLTmxMap();

	map->width = atoi(mapnode->first_attribute("width")->value());
	map->height = atoi(mapnode->first_attribute("height")->value());
	map->tileWidth = atoi(mapnode->first_attribute("tilewidth")->value());
	map->tileHeight = atoi(mapnode->first_attribute("tileheight")->value());

	auto tilesetnode = mapnode->first_node("tileset");

	while (tilesetnode)
	{
		auto tileset = new NLTmxMapTileset();

		tileset->firstGid = atoi(tilesetnode->first_attribute("firstgid")->value());
		tileset->name = tilesetnode->first_attribute("name")->value();
		tileset->tileWidth = atoi(tilesetnode->first_attribute("tilewidth")->value());
		tileset->tileHeight = atoi(tilesetnode->first_attribute("tileheight")->value());
		tileset->filename = tilesetnode->first_node("image")->first_attribute("source")->value();

		//cout << "Tileset " << tileset->name << " filename " << tileset->filename << endl;

		map->tilesets.push_back(tileset);

		tilesetnode = tilesetnode->next_sibling("tileset");
	}

	const auto separators = " \t,\n\r";

	auto layernode = mapnode->first_node("layer");

	while (layernode)
	{
		auto layer = new NLTmxMapLayer();

		layer->name = layernode->first_attribute("name")->value();
		layer->width = atoi(layernode->first_attribute("width")->value());
		layer->height = atoi(layernode->first_attribute("height")->value());

		const char* data = layernode->first_node("data")->value();

		assert(strcmp(data, "csv"));

		layer->data = new int[layer->width * layer->height];

		const auto copy = static_cast<char*>(malloc(strlen(data) + 1));
		strcpy(copy, data);
		auto item = strtok(copy, separators);

		auto index = 0;
		while (item)
		{
			layer->data[index] = atoi(item);
			index++;

			item = strtok(nullptr, separators);
		}

		free(copy);

		map->layers.push_back(layer);

		layernode = layernode->next_sibling("layer");
	}

	auto objectgroupnode = mapnode->first_node("objectgroup");

	while (objectgroupnode)
	{
		auto group = new NLTmxMapObjectGroup();

		group->name = objectgroupnode->first_attribute("name")->value();
		//group->width = atoi( objectgroupnode->first_attribute( "width" )->value() );
		//group->height = atoi( objectgroupnode->first_attribute( "height" )->value() );

		const auto visible_attribute = objectgroupnode->first_attribute("visible");
		if (visible_attribute)
		{
			group->visible = (atoi(visible_attribute->value()) != 0);
		}
		else
		{
			group->visible = true;
		}

		//cout << "group " << group->name << endl;

		auto objectnode = objectgroupnode->first_node("object");

		while (objectnode)
		{
			auto object = new NLTmxMapObject();

			const auto name_attribute = objectnode->first_attribute("name");
			if (name_attribute)
			{
				object->name = name_attribute->value();
			}
			const auto id_attribute = objectnode->first_attribute("id");
			if (id_attribute)
			{
				object->id = atoi(id_attribute->value());
			}
			const auto gid_attribute = objectnode->first_attribute("gid");
			if (gid_attribute)
			{
				object->gid = atoi(gid_attribute->value());
			}
			const auto type_attribute = objectnode->first_attribute("type");
			if (type_attribute)
			{
				object->type = type_attribute->value();
			}
			object->x = atoi(objectnode->first_attribute("x")->value());
			object->y = atoi(objectnode->first_attribute("y")->value());

			const auto width_attribute = objectnode->first_attribute("width");
			if (width_attribute)
			{
				object->width = atoi(width_attribute->value());
			}

			const auto height_attribute = objectnode->first_attribute("height");
			if (height_attribute)
			{
				object->height = atoi(height_attribute->value());
			}

			const auto rotation_attribute = objectnode->first_attribute("rotation");
			if (rotation_attribute)
			{
				object->rotation = stof(rotation_attribute->value());
			}

			const auto properties_node = objectnode->first_node("properties");

			if (properties_node)
			{
				auto property_node = properties_node->first_node("property");

				while (property_node)
				{
					auto property = new NLTmxMapObjectProperty();
					property->name = property_node->first_attribute("name")->value();
					property->value = property_node->first_attribute("value")->value();

					object->properties.push_back(property);

					property_node = property_node->next_sibling("property");
				}
			}

			group->objects.push_back(object);

			objectnode = objectnode->next_sibling("object");
		}

		map->groups.push_back(group);

		objectgroupnode = objectgroupnode->next_sibling("objectgroup");
	}

	//free( (void*) xml );

	return map;
}
