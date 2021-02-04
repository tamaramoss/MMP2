#include "stdafx.h"

#include "GameObject.h"
#include "PlayerMoveComponent.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include <iostream>

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
	m_rigidBody.getB2Body()->SetGravityScale(850);
	return true;
}

void
PlayerMoveComponent::update(const float deltaTime)
{
	const auto speed = 10000.0f; // pixels/second 
	Vector2f translation{};
	/*if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
		translation.x += speed * deltaTime;
	if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
		translation.x -= speed * deltaTime;
	if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
		translation.y -= speed * deltaTime;
	if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
		translation.y += speed * deltaTime;*/

	//sf::Vector2f input = InputManager::getInstance().getAxisPosition("Joystick", m_playerIndex);
	//translation = input;
	if (m_playerIndex == 1)
		std::cout << translation.x << " " << translation.y << std::endl;

	if (translation.x == 0 && translation.y == 0)
	{
		auto vel = m_rigidBody.getB2Body()->GetLinearVelocity();
		vel = PhysicsManager::s2b(Vector2f(-vel.x * 2.5f, 0));
		m_rigidBody.getB2Body()->ApplyForce(vel, b2Vec2(0,0), true);
		m_rigidBody.getB2Body()->SetGravityScale(850);

	}
	else
	{
		//m_rigidBody.getB2Body()->SetAngularDamping(50.f);
		m_rigidBody.getB2Body()->SetGravityScale(0);
	}

#if 1 // physics movement
	m_rigidBody.addVelocity(translation * speed * deltaTime);
#else // no physics movement
	mGameObject.GetTransform().translate(translation);
#endif
}
