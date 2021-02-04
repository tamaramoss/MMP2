#include "stdafx.h"
#include "SlimeBallComponent.h"
#include "GameObject.h"
#include <iostream>

SlimeBallComponent::SlimeBallComponent(GameObject& gameObject) : IGameComponent(gameObject)
{
}

bool SlimeBallComponent::init()
{
	return true;
}

void SlimeBallComponent::update(float fDeltaTime)
{
}

void SlimeBallComponent::onCollision(ColliderComponent& other)
{
	/*if(other.getGameObject().getTag() == "Hand")
		std::cout << "AAAAA" << std::endl;*/
}
