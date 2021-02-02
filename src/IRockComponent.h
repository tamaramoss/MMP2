#pragma once

#include "IGameComponent.h"
#include "HandMoveComponent.h"

class IRockComponent : public IGameComponent
{
public:
	IRockComponent(GameObject& gameObject) : IGameComponent(gameObject) {}
	virtual void grabRock() = 0;
	virtual void releaseRock() = 0;
	virtual void setHand(std::shared_ptr<HandMoveComponent> hand) = 0;

protected:
	bool mIsGrabbed = false;
	std::shared_ptr<HandMoveComponent> mHandReference;
};