#pragma once

#include "IRockComponent.h"

class RockNormalComponent : public IRockComponent
{
public:
	RockNormalComponent(GameObject& gameObject);
	virtual bool init() override;
	virtual void update(float fDeltaTime) override;
	virtual void grabRock() override;
	virtual void releaseRock() override;

	// Inherited via IRockComponent
	virtual void setHand(std::shared_ptr<HandMoveComponent> hand) override;
};