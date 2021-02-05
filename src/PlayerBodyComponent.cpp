#include "stdafx.h"

#include "GameObject.h"
#include "PhysicsManager.h"
#include <iostream>
#include "PlayerBodyComponent.h"
#include "SoundComponent.h"

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

void PlayerBodyComponent::onCollision(ColliderComponent& other)
{
	if (other.getGameObject().getTag() == "Slime")
	{
		for (auto h : mHands)
		{
			h->setReleaseFlag(true);
			h->getGameObject().get_component<RigidBodyComponent>()->getB2Body()->ApplyLinearImpulse(b2Vec2(0, 3000), b2Vec2(0,0), true);
		}

		other.getGameObject().setScale(0,0);
		other.getBody().getB2Body()->SetLinearVelocity(-other.getBody().getB2Body()->GetLinearVelocity());

		mGameObject.get_component<SoundComponent>()->setSound("Hit");
	}
}

void PlayerBodyComponent::addHand(std::shared_ptr<HandMoveComponent> hand)
{
	mHands.push_back(hand);
}
