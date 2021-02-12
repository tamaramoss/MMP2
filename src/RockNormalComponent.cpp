/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "RockNormalComponent.h"

#include "AnimationComponent.h"
#include "GameObject.h"

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
	//mGameObject.get_component<AnimationComponent>()->setAnimation("Grabbed");
}

void RockNormalComponent::releaseRock()
{
	//mGameObject.get_component<AnimationComponent>()->setAnimation("Default");
}

void RockNormalComponent::setHand(std::shared_ptr<HandMoveComponent> hand)
{
}
