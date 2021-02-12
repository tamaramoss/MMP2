/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "ConstantVelocityComponent.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "PhysicsManager.h"

ConstantVelocityComponent::ConstantVelocityComponent(GameObject& gameObject, sf::Vector2f velocity) : 
    IGameComponent(gameObject),
    mVelocity(velocity)
{

}

bool ConstantVelocityComponent::init()
{
    return true;
}

void ConstantVelocityComponent::update(float fDeltaTime)
{
    mGameObject.get_component<RigidBodyComponent>()->getB2Body()->SetLinearVelocity(PhysicsManager::s2b(mVelocity));
}
