#include "stdafx.h"
#include "SpitterComponent.h"
#include <iostream>
#include "RigidBodyComponent.h"
#include "PhysicsManager.h"
#include "AnimationComponent.h"

SpitterComponent::SpitterComponent(GameObject& gameObject) : IGameComponent(gameObject)
{
}

bool SpitterComponent::init()
{
	return true;
}

void SpitterComponent::update(float fDeltaTime)
{
	if (!mSpittingAtm)
		return;

	mTimer += fDeltaTime;
	if (mTimer >= mTimeInterval)
	{
		spitSlime();
		mTimer = 0;
	}
}

void SpitterComponent::startSpitting()
{
	if(!mSpittingAtm)
		mSpittingAtm = true;
}

void SpitterComponent::stopSpitting()
{
	mSpittingAtm = false;
}

void SpitterComponent::addSlime(GameObject::ptr slime)
{
	mSlimes.push_back(slime);
	mTotalSlimes++;
}

void SpitterComponent::setTimeInterval(float sec)
{
	mTimeInterval = sec;
}

void SpitterComponent::spitSlime()
{
	mGameObject.get_component<AnimationComponent>()->setAnimation("Spitting");

	mCurrentIndex++;
	if (mCurrentIndex == mTotalSlimes)
	{
		mCurrentIndex = 0;
	}

	auto curSlime = mSlimes[mCurrentIndex];
	mSlimes[mCurrentIndex]->get_component<RigidBodyComponent>()->getB2Body()->SetTransform(PhysicsManager::s2b(mGameObject.getPosition() + sf::Vector2f(80, 300)), 0);
	mSlimes[mCurrentIndex]->get_component<RigidBodyComponent>()->getB2Body()->SetLinearVelocity(b2Vec2(-100.f * (mIsMirrored ? -1 : 1), 0));
	mSlimes[mCurrentIndex]->setScale(1,1);
}

void SpitterComponent::setMirrored(bool mirrored)
{
	mIsMirrored = mirrored;
}

void SpitterComponent::setSlimeKinematic(bool kinematic)
{
	mSlimeIsKinematic = kinematic;
}

void SpitterComponent::setSpitterIndex(int index)
{
	mSpitterIndex = index;
}

int SpitterComponent::getSpitterIndex()
{
	return mSpitterIndex;
}
