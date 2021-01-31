#include "stdafx.h"

#include "HandMoveComponent.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include <iostream>



HandMoveComponent::HandMoveComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, int playerIndex, bool useRightStick)
	:IGameComponent(gameObject),
	mRigidBody(rigidBody),
	mPlayerIndex(playerIndex),
	mUseRightStick(useRightStick)
{
}

bool HandMoveComponent::init()
{
	auto body = mRigidBody.getB2Body();
	//body->SetGravityScale(850);
	body->SetFixedRotation(true);
	return false;
}

void HandMoveComponent::update(float deltaTime)
{
	sf::Vector2f translation = InputManager::getInstance().getAxisPosition(mUseRightStick, mPlayerIndex);


	if ((translation.x == 0 && translation.y == 0))
	{
		mRigidBody.getB2Body()->SetGravityScale(850);
	}
	else
	{
		mRigidBody.getB2Body()->SetGravityScale(0);
	}

	move(translation, mMoveSpeed * 100);

	// grabbing
	if (InputManager::getInstance().isButtonPressed(mUseRightStick ? "GrabRight" : "GrabLeft"))
	{
		grab();
	}

	if (InputManager::getInstance().isButtonReleased(mUseRightStick ? "GrabRight" : "GrabLeft"))
	{
		release();
	}
}

void HandMoveComponent::setJointLength(float length)
{
}

void HandMoveComponent::setJointPullLength(float length)
{
}

void HandMoveComponent::grab()
{
	std::cout << "grab" << std::endl;
	mIsGrabbing = true;
	mRigidBody.getB2Body()->SetType(b2_staticBody);
	mGrabPosition = PhysicsManager::b2s(mRigidBody.getB2Body()->GetPosition());
}

void HandMoveComponent::release()
{
	std::cout << "release" << std::endl;
	mRigidBody.getB2Body()->SetType(b2_dynamicBody);

	mIsGrabbing = false;
}

void HandMoveComponent::pullUp()
{
}

void HandMoveComponent::extend()
{
}

void HandMoveComponent::move(sf::Vector2f direction, float speed)
{
	auto body = mRigidBody.getB2Body();
	body->ApplyForce(PhysicsManager::s2b(direction * speed), b2Vec2(0, 0), false);
}

