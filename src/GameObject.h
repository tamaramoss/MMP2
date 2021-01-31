#pragma once

#pragma once

#include <list>
#include <string>

#include <utility>

#include "IGameComponent.h"

class GameObject : public Transformable
{
public:
	using ptr = std::shared_ptr<GameObject>;

	GameObject(std::string id, std::string tag)
		: mId(std::move(id)),
		mTag(std::move(tag))
	{
	}

	/// Add a component to the game object. Internally the game object
	/// identifies if it's a drawable component to call it's draw
	/// function or a general update component to call it's update
	/// function.
	/// \param component the component
	void add_component(const IGameComponent::ptr& component);

	/// Returns the component of the given type
	template <typename TComponent>
	std::shared_ptr<TComponent> get_component()
	{
		for (auto component : mComponentsList)
		{
			std::dynamic_pointer_cast<TComponent>(component);
			if (std::shared_ptr<TComponent> cmp = std::dynamic_pointer_cast<TComponent>(component))
			{
				return cmp;
			}
		}
		return nullptr;
	}

	/// Initialize all added components
	bool init();

	/// Could also be moved out of game object in the long run.
	void update(float deltaTime);

	// TODO delete flag not used, i.e., objects are not deleted
	bool isMarkedForDelete() const { return mWantToDie; }
	void markForDelete() { mWantToDie = true; }

	std::string getId() const { return mId; }
	void setId(const std::string& id) { mId = id; }

	std::string getTag() const { return mTag; }
	void setTag(const std::string& tag) { mTag = tag; }

protected:
	std::string mId = "unnamed"; //< unique name of object, e.g., player
	std::string mTag = "untagged";
	bool mWantToDie = false;

	std::list<IGameComponent::ptr> mComponentsList;
};
