#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "IRenderComponent.h"

struct Tile
{
	// original tile index
	int idx;
	std::shared_ptr<Sprite> sprite;
};

struct TileLayer
{
	std::string name;
	Vector2i dimension;
	Vector2i tileSize;
	std::vector<Tile> tiles;
};

class TileLayerRenderComponent : public IRenderComponent
{
public:
	using ptr = std::shared_ptr<TileLayerRenderComponent>;

	TileLayerRenderComponent(GameObject& gameObject,
	                         RenderWindow& renderWindow,
	                         TileLayer layer);

	bool init() override;
	void update(float deltaTime) override;
	void draw() override;

	const TileLayer& getLayer() const { return mLayer; }

private:

	TileLayer mLayer;
};
