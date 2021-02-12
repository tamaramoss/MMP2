/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once
#include "IGameComponent.h"
#include <SFML\System\Vector2.hpp>

class ConstantVelocityComponent : public IGameComponent
{
public:
	ConstantVelocityComponent(GameObject& gameObject, sf::Vector2f velocity);

	virtual bool init() override;
	virtual void update(float fDeltaTime) override;

private:
	sf::Vector2f mVelocity;

};