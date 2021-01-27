#include "stdafx.h"

#include "GameObject.h"
#include "PlayerMoveComponent.h"
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
	const auto speed = 1'000.0f; // pixels/second 
	Vector2f translation{};
	if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
		translation.x += speed * deltaTime;
	if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
		translation.x -= speed * deltaTime;
	if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
		translation.y -= speed * deltaTime;
	if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
		translation.y += speed * deltaTime;

#if 1 // physics movement
	m_rigidBody.addVelocity(translation);
#else // no physics movement
	mGameObject.GetTransform().translate(translation);
#endif
}
