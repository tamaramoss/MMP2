#pragma once

#include "IRockComponent.h"
#include "HandMoveComponent.h"

class RockTimedComponent : public IRockComponent 
{
public:
	RockTimedComponent(GameObject& gameObject);
	virtual bool init() override;
	virtual void update(float fDeltaTime) override;
	virtual void grabRock() override;
	virtual void releaseRock() override;
	virtual void setHand(std::shared_ptr<HandMoveComponent> hand) override;

private:
	float mTimer = 0;
	float mTimeUntilDestroyed = 5;

	void destroyRock();
};