#include "stdafx.h"

#include "Debug.h"

#include "GameObjectManager.h"
#include "GameObjectEvents.h"
#include "SpriteManager.h"
#include "RigidBodyComponent.h"

void GameObjectManager::init()
{
	// subscribe to creation events
	{
		const EventBus::ListenerId id = EventBus::getInstance().addListener(GameObjectCreateEvent::eventType,
		                                                                       [this](const IEvent::ptr event)
		                                                                       {
			                                                                       auto goCreateEvent = std::
				                                                                       static_pointer_cast<
					                                                                       GameObjectCreateEvent
				                                                                       >(event);
			                                                                       this->addGameObject(
				                                                                       goCreateEvent->getData());
		                                                                       });
		mListeners.push_back(id);
	}
}

void GameObjectManager::shutdown()
{
	mGameObjects.clear();

	// unsubscribe from events
	for (auto& m_listener : mListeners)
		EventBus::getInstance().removeListener(m_listener);
	mListeners.clear();
}

void
GameObjectManager::addGameObject(const GameObject::ptr& gameObject)
{
	FF_ASSERT_MSG(mGameObjects.find(gameObject->getId()) == mGameObjects.end(),
		"Game object with this m_id already exists " + gameObject->getId());

	mGameObjects[gameObject->getId()] = gameObject;
}

GameObject::ptr GameObjectManager::getGameObject(const std::string& id) const
{
	const auto it = mGameObjects.find(id);
	if (it == mGameObjects.end())
	{
		FF_ERROR_MSG("Could not find gameobject with m_id " + id);
		return nullptr;
	}
	return it->second;
}

void GameObjectManager::removeGameObject(const std::string& id)
{
	const auto it = mGameObjects.find(id);
	if (it == mGameObjects.end())
	{
		FF_ERROR_MSG("Could not find gameobject with m_id " + id);
		return;
	}
	
	//mGameObjects.erase(it);
}
