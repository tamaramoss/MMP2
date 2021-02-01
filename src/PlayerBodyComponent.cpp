#include "stdafx.h"

#include "GameObject.h"
#include "PlayerMoveComponent.h"
#include "PhysicsManager.h"
#include <iostream>
#include "PlayerBodyComponent.h"

PlayerBodyComponent::PlayerBodyComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, float armLength)
	:IGameComponent(gameObject),
	mArmLength(armLength),
	mRigidbody(rigidBody)
{
}

bool PlayerBodyComponent::init()
{
	mRigidbody.getB2Body()->SetGravityScale(850);
	return true;
}

void PlayerBodyComponent::update(float deltaTime)
{
}

