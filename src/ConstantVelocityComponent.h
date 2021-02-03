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