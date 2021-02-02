#pragma once
#include <utility>


#include "GameObject.h"


class SpitterTriggerComponent
{
public:
	SpitterTriggerComponent(GameObject::ptr spitter) : mSpitter(spitter) { }
	
	void setSpitter(GameObject::ptr spitter) { mSpitter = std::move(spitter); }
	GameObject& getSpitter() { return *mSpitter; }
	
private:
	GameObject::ptr mSpitter;
};
