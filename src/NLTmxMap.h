/// modified by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

// TMX map file parser from https://github.com/catnapgames/TestNLTmxMap
// modification: getTilesetForGrid added (WL)

#include <string>
#include <vector>

using namespace std;

class NLTmxMapTileset
{
public:
	int firstGid{};
	string name;
	int tileWidth{};
	int tileHeight{};
	string filename;
};


class NLTmxMapLayer
{
public:
	string name;
	int width{};
	int height{};
	int* data{};

	~NLTmxMapLayer()
	{
		delete[] data;
	}
};


struct NLTmxMapObjectProperty
{
	string name;
	string value;
};


class NLTmxMapObject
{
public:
	string name;
	string type;
	int gid{};
	int id{};
	int x{};
	int y{};
	int width{};
	int height{};
	float rotation{};
	vector<NLTmxMapObjectProperty*> properties;

public:

	~NLTmxMapObject()
	{
		for (auto property : properties)
		{
			delete property;
		}
	}
};


class NLTmxMapObjectGroup
{
public:
	string name;
	int width{};
	int height{};
	bool visible{};

	vector<NLTmxMapObject*> objects;

	~NLTmxMapObjectGroup()
	{
		for (auto o : objects)
		{
			delete o;
		}
	}
};


class NLTmxMap
{
public:

	int width{};
	int height{};
	int tileWidth{};
	int tileHeight{};

	vector<NLTmxMapTileset*> tilesets;
	vector<NLTmxMapLayer*> layers;
	vector<NLTmxMapObjectGroup*> groups;

	~NLTmxMap()
	{
		for (auto g : groups)
		{
			delete g;
		}

		for (auto l : layers)
		{
			delete l;
		}

		for (auto ts : tilesets)
		{
			delete ts;
		}
	}

	NLTmxMapTileset* get_tileset_for_grid(int grid);
};


NLTmxMap* NLLoadTmxMap(char* xml);

NLTmxMap* NLLoadTmxMap(const std::string& filename);
