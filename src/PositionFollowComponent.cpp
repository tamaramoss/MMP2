/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "PositionFollowComponent.h"
#include "GameObject.h"

PositionFollowComponent::PositionFollowComponent(GameObject& gameObject, sf::Vector2f position)
	: IGameComponent(gameObject),
	mFollowPosition(position)
{
}

void PositionFollowComponent::setFollowPosition(sf::Vector2f position)
{
	mFollowPosition = position;
}

sf::Vector2f PositionFollowComponent::getFollowPosition()
{
	return mFollowPosition;
}

bool PositionFollowComponent::init()
{
	return true;
}

void PositionFollowComponent::update(float fDeltaTime)
{
	mGameObject.setPosition(mFollowPosition);
}
