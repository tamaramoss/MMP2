/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "RockTimedComponent.h"
#include "AnimationComponent.h"
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
	if (mWasGrabbed)
	{
		if (!mGameObject.get_component<AnimationComponent>()->isPlaying())
		{
			mGameObject.get_component<AnimationComponent>()->setAnimation("Angry");
			mGameObject.get_component<AnimationComponent>()->setFrameTime(0.1f);
			mGameObject.get_component<AnimationComponent>()->setLooped(true);
		}
		
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
	if (!mWasGrabbed)
	{
		mGameObject.get_component<AnimationComponent>()->setAnimation("Wake");
		mGameObject.get_component<AnimationComponent>()->setLooped(false);
	}
	
	mIsGrabbed = true;
	mWasGrabbed = true;
	
}

void RockTimedComponent::releaseRock()
{
	mIsGrabbed = false;
}

void RockTimedComponent::destroyRock()
{
	/*mGameObject.markForDelete();*/
	mGameObject.get_component<RigidBodyComponent>()->getB2Body()->SetTransform(b2Vec2(-10000000, -10000000), 0);

	if(mIsGrabbed)
		mHandReference->release();

}

void RockTimedComponent::setHand(std::shared_ptr<HandMoveComponent> hand)
{
	mHandReference = hand;
}
