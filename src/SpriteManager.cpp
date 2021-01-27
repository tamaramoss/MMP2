#include "stdafx.h"

#include "SpriteManager.h"
#include "IRenderComponent.h"

#include "RenderComponentEvents.h"

void SpriteManager::init()
{
	// subscribe to creation events
	{
		const auto id = EventBus::getInstance().addListener(
			RenderableCreateEvent::m_eventType,
			[this](const IEvent::ptr event)
			{
				auto renderable_create_event = std::
					static_pointer_cast<RenderableCreateEvent>(event);
				this->addCompToLayer(
					renderable_create_event->getLayer(),
					&renderable_create_event->getData());
			});
		mListeners.push_back(id);
	}
}

void SpriteManager::shutdown()
{
	mLayerOrder.clear();
	mLayerToComp.clear();
	mCompToLayer.clear();
	mTileSets.clear();
	mCamera = nullptr;

	// unsubscribe from events
	for (auto& m_listener : mListeners)
		EventBus::getInstance().removeListener(m_listener);
	mListeners.clear();
}

void
SpriteManager::addCompToLayer(const std::string& layer, IRenderComponent* comp)
{
	if (mCompToLayer.find(comp) != mCompToLayer.end())
	{
		FF_ERROR_MSG("Removing component from layer, before adding to new layer. from "
			+ mCompToLayer[comp] + " to " + layer);

		mLayerToComp[layer].remove(comp);
	}

	mCompToLayer[comp] = layer;
	mLayerToComp[layer].push_back(comp);
}

void
SpriteManager::removeComp(IRenderComponent* comp)
{
	const auto it = mCompToLayer.find(comp);
	if (it == mCompToLayer.end())
		return;

	mLayerToComp[it->second].remove(comp);
	mCompToLayer.erase(it);
}

std::string
SpriteManager::getLayer(IRenderComponent* comp)
{
	const auto it = mCompToLayer.find(comp);
	if (it != mCompToLayer.end())
		return it->second;
	return {};
}

void
SpriteManager::draw()
{
	FF_ASSERT_MSG(mCamera != nullptr, "Camera is nullptr");

	mCamera->draw();

	for (const auto& order : mLayerOrder)
	{
		FF_ASSERT_MSG(mLayerToComp.find(order) != mLayerToComp.end(),
			"Layer with this name not defined: " + order);

		auto& layer = mLayerToComp[order];
		for (auto comp : layer)
			comp->draw();
	}
}
