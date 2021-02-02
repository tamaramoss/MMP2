#include "stdafx.h"

#include "GameObject.h"
#include "PlayerMoveComponent.h"
#include "PhysicsManager.h"
#include <iostream>
#include "PlayerBodyComponent.h"

PlayerBodyComponent::PlayerBodyComponent(GameObject& gameObject, RigidBodyComponent& rigidBody)
	:IGameComponent(gameObject),
	mRigidbody(rigidBody)
{
}

bool PlayerBodyComponent::init()
{
	mRigidbody.getB2Body()->SetGravityScale(1050);
	mRigidbody.getB2Body()->SetFixedRotation(true);
	mRigidbody.getB2Body()->SetSleepingAllowed(false);

	return true;
}

void PlayerBodyComponent::update(float deltaTime)
{

}
