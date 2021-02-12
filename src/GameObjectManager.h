#pragma once

#include <string>
#include <unordered_map>

#include "GameObject.h"

#include "EventBus.h"


class GameObjectManager
{
public:
	using GameObjectMap = std::unordered_map<std::string, GameObject::ptr>;
	static GameObjectManager& getInstance();

	void init();
	void reset();
	void shutdown();

	void addGameObject(const GameObject::ptr& gameObject);
	GameObject::ptr getGameObject(const std::string& id) const;

	GameObjectMap& getGameObjects() { return mGameObjects; }

	void removeGameObject(const std::string& id);

private:

	GameObjectMap mGameObjects;

	// maintain a list of listeners that the manager is subscribed to,
	// so that he can unsubscribe
	std::list<EventBus::ListenerId> mListeners;
};
