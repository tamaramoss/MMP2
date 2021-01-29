#pragma once

#include <memory>

class GameObject;

class IGameComponent
{
public:
	using ptr = std::shared_ptr<IGameComponent>;

	IGameComponent(GameObject& gameObject)
		: mGameObject(gameObject)
	{
	}

	virtual ~IGameComponent() = default;

	virtual bool init() = 0;
	virtual void update(float fDeltaTime) = 0;
	GameObject& getGameObject() const { return mGameObject; }

protected:
	GameObject& mGameObject;
};