#include "stdafx.h"

#include "GameObject.h"
#include "PlayerMoveComponent.h"

#include <iostream>

#include "InputManager.h"

PlayerMoveComponent::PlayerMoveComponent(
	GameObject& gameObject, RigidBodyComponent& rigidBody, const int playerIndex)
	: IGameComponent(gameObject)
	  , m_playerIndex(playerIndex)
	  , m_rigidBody(rigidBody)
{
}

bool
PlayerMoveComponent::init()
{
	return true;
}

void
PlayerMoveComponent::update(const float deltaTime)
{
	const auto speed = 100.0f; // pixels/second 
	Vector2f translation{};
	//if (InputManager::getInstance().isButtonDown("right", m_playerIndex))
	//	translation.x += speed * deltaTime;
	//if (InputManager::getInstance().isButtonDown("left", m_playerIndex))
	//	translation.x -= speed * deltaTime;
	//if (InputManager::getInstance().isButtonDown("up", m_playerIndex))
	//	translation.y -= speed * deltaTime;
	//if (InputManager::getInstance().isButtonDown("down", m_playerIndex))
	//	translation.y += speed * deltaTime;
	//

	const sf::Vector2f axisPosition = InputManager::getInstance().getDpadPosition(m_playerIndex);
	const float zAxisPosition = InputManager::getInstance().getTriggerPosition(m_playerIndex);
	std::cout << axisPosition.x << " " << axisPosition.y << std::endl;
	std::cout << zAxisPosition << std::endl;
	translation.x += axisPosition.x * speed * deltaTime;
	translation.y += axisPosition.y * speed * deltaTime;
	

#if 1 // physics movement
	m_rigidBody.addVelocity(translation);
#else // no physics movement
	mGameObject.GetTransform().translate(translation);
#endif
}
