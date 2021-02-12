/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "RockOneTimeComponent.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"
#include "SoundComponent.h"

RockOneTimeComponent::RockOneTimeComponent(GameObject& gameObject)
    : IRockComponent(gameObject)
{
}

bool RockOneTimeComponent::init()
{
    return true;
}

void RockOneTimeComponent::update(float fDeltaTime)
{
}

void RockOneTimeComponent::grabRock()
{
    mIsGrabbed = true;
}

void RockOneTimeComponent::releaseRock()
{
    mIsGrabbed = false;
    mGameObject.get_component<RigidBodyComponent>()->getB2Body()->SetTransform(b2Vec2(-10000000, -10000000), 0);
    mGameObject.setPosition(sf::Vector2f(-100000000, -100000000));
    mGameObject.get_component<SoundComponent>()->setSound("Dead");
}

void RockOneTimeComponent::setHand(std::shared_ptr<HandMoveComponent> hand)
{
    
}
