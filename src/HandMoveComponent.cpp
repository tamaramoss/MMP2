#include "stdafx.h"

#include "HandMoveComponent.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <iostream>
#include "VectorAlgebra2D.h"



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
	body->SetType(b2_dynamicBody);
	body->SetFixedRotation(true);
	return true;
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
	if (InputManager::getInstance().isButtonPressed(mUseRightStick ? "GrabRight" : "GrabLeft") && mCanGrab && !mIsGrabbing)
	{
		grab();
	}

	if (InputManager::getInstance().isButtonReleased(mUseRightStick ? "GrabRight" : "GrabLeft") && mIsGrabbing)
	{
		release();
	}

	if (mCanGrab)
	{
		float distance = MathUtil::length(mGrabPosition - mGameObject.getPosition());
		if (distance > 400.f)
		{
			std::cout << distance << std::endl;
			mCanGrab = false;
		}
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
	// if distance to body >= maxDistance and the other hand is not holding on, then no more force.
	// otherwise add force
	auto hand = mRigidBody.getB2Body();

	float distanceToBody = PhysicsManager::UNRATIO * MathUtil::length(mBody->getPosition() - mGameObject.getPosition());
	if (distanceToBody > mNormalLength - 1.5f && !mOtherHand->mIsGrabbing)
	{
		direction = (mBody->getPosition() - mGameObject.getPosition()) / MathUtil::length(mBody->getPosition() - mGameObject.getPosition());
	}

	hand->ApplyForce(PhysicsManager::s2b(direction * speed), b2Vec2(0, 0), false);
}

void HandMoveComponent::onCollisionEnter(ColliderComponent& other)
{
	if (other.getGameObject().getTag() == "Grabbable")
	{
		std::cout << "rocc" << std::endl;
		mGrabPosition = other.getGameObject().getPosition();
		mCanGrab = true;
	}
}