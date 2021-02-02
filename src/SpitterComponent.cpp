#include "stdafx.h"
#include "SpitterComponent.h"

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

	
}

void SpitterComponent::startSpitting()
{
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
