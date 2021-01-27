#include "stdafx.h"

#include "GameObject.h"
#include "TileLayerRenderComponent.h"
#include <utility>

TileLayerRenderComponent::TileLayerRenderComponent(
	GameObject& gameObject,
	RenderWindow& renderWindow,
	TileLayer layer)
	: IRenderComponent(gameObject, renderWindow)
	  , mLayer(std::move(layer))
{
}

bool TileLayerRenderComponent::init()
{
	return true;
}

void TileLayerRenderComponent::update(float deltaTime)
{
	
}

void TileLayerRenderComponent::draw()
{
	for (auto& tile : mLayer.tiles)
		mRenderWindow.draw(*(tile.sprite));
}
