/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "GameObject.h"

void
GameObject::add_component(const IGameComponent::ptr& component)
{
	mComponentsList.push_back(component);
}

void GameObject::update(const float deltaTime)
{
	for (const auto& comp : mComponentsList)
		comp->update(deltaTime);
}

bool GameObject::init()
{
	for (const auto& comp : mComponentsList)
	{
		if (!comp->init())
		{
			err() << "Could not initialize component of object " << mId << std::endl;
			return false;
		}
	}
	return true;
}
