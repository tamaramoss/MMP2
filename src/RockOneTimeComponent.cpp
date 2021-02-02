#include "stdafx.h"

#include "RockOneTimeComponent.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"

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
}

void RockOneTimeComponent::setHand(std::shared_ptr<HandMoveComponent> hand)
{
    
}
