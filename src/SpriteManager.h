/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "EventBus.h"

class IRenderComponent;

class SpriteManager
{
public:

	typedef std::unordered_map<std::string, std::shared_ptr<Texture>> TileSetMap;

	//
	SpriteManager(RenderWindow& renderWindow)
		: mRenderWindow(renderWindow)
	{
	}

	void init();
	void shutdown();

	/// Set camera component
	void setCamera(IRenderComponent* cameraComp) { mCamera = cameraComp; }
	IRenderComponent* getCamera() const { return mCamera; }

	/// Add component to the named layer. If the component is already in a layer
	/// it is removed from this layer and put in this layer.
	void addCompToLayer(const std::string& layer, IRenderComponent* comp);
	void removeComp(IRenderComponent* comp);

	/// Get current layer of the given component.
	std::string getLayer(IRenderComponent* comp);

	/// Set up the order of layers by providing their names in order
	/// from back to front.
	void setLayerOrder(const std::vector<std::string>& layerOrder)
	{
		mLayerOrder = layerOrder;
	}

	//
	void setTileSize(const Vector2i& tileSize) { mTileSize = tileSize; }
	Vector2i getTileSize() const { return mTileSize; }

	//
	void setTileSets(TileSetMap& tileSets)
	{
		mTileSets = tileSets;
	}

	const TileSetMap& getTileSets() const { return mTileSets; }

	//
	RenderWindow& getWindow() const { return mRenderWindow; }

	//
	void draw();

private:

	RenderWindow& mRenderWindow;

	Vector2i mTileSize{};
	TileSetMap mTileSets;

	IRenderComponent* mCamera = nullptr;

	std::map<std::string, std::list<IRenderComponent*>> mLayerToComp;
	std::map<IRenderComponent*, std::string> mCompToLayer;

	std::vector<std::string> mLayerOrder;

	// maintain a list of listeners that the manager is subscribed to,
	// so that he can unsubscribe
	std::list<EventBus::ListenerId> mListeners;
};
