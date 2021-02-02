#include "stdafx.h"

#include "RockNormalComponent.h"

RockNormalComponent::RockNormalComponent(GameObject& gameObject)
	:IRockComponent(gameObject)
{
}

bool RockNormalComponent::init()
{
	return true;
}

void RockNormalComponent::update(float fDeltaTime)
{
}

void RockNormalComponent::grabRock()
{
}

void RockNormalComponent::releaseRock()
{
}

void RockNormalComponent::setHand(std::shared_ptr<HandMoveComponent> hand)
{
}
