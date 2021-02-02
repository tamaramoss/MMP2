#include "stdafx.h"

#include "RockTimedComponent.h"
#include "GameObjectManager.h"
#include "RigidBodyComponent.h"

RockTimedComponent::RockTimedComponent(GameObject& gameObject)
	: IRockComponent(gameObject)
{
}

bool RockTimedComponent::init()
{
	return true;
}

void RockTimedComponent::update(float fDeltaTime)
{
	if (mIsGrabbed)
	{
		if (mTimer < mTimeUntilDestroyed)
		{
			mTimer += fDeltaTime;
		}
		else
		{
			destroyRock();
		}
	}
}

void RockTimedComponent::grabRock()
{
	mIsGrabbed = true;
}

void RockTimedComponent::releaseRock()
{
	//mIsGrabbed = false;
}

void RockTimedComponent::destroyRock()
{
	/*mGameObject.markForDelete();*/
	mIsGrabbed = false;
	mGameObject.get_component<RigidBodyComponent>()->getB2Body()->SetTransform(b2Vec2(-10000000, -10000000), 0);
	mHandReference->release();

}

void RockTimedComponent::setHand(std::shared_ptr<HandMoveComponent> hand)
{
	mHandReference = hand;
}
