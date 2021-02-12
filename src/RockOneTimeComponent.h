/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once
#include "IRockComponent.h"
#include "HandMoveComponent.h"

class RockOneTimeComponent : public IRockComponent
{
public:
	RockOneTimeComponent(GameObject& gameObject);

	virtual bool init() override;
	virtual void update(float fDeltaTime) override;
	virtual void grabRock() override;
	virtual void releaseRock() override;
	virtual void setHand(std::shared_ptr<HandMoveComponent> hand) override;

};