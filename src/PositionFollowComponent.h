/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once
#include <SFML\System\Vector2.hpp>
#include "IGameComponent.h"

class PositionFollowComponent : public IGameComponent
{
public:
	PositionFollowComponent(GameObject& gameObject, sf::Vector2f position);
	
	virtual bool init() override;
	virtual void update(float fDeltaTime) override;

	void setFollowPosition(sf::Vector2f position);
	sf::Vector2f getFollowPosition();

private:
	sf::Vector2f mFollowPosition;
};