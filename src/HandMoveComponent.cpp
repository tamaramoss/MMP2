#include "stdafx.h"

#include "HandMoveComponent.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <iostream>
#include "VectorAlgebra2D.h"
#include "PlayerBodyComponent.h"
#include "IRockComponent.h"



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
	body->SetType(b2_dynamicBody);
	body->SetFixedRotation(true);
	body->SetSleepingAllowed(false);
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

	// grabbing
	if (InputManager::getInstance().isButtonPressed(mUseRightStick ? "GrabRight" : "GrabLeft") && mCanGrab && !mIsGrabbing)
	{
		grab();
	}

	if (InputManager::getInstance().isButtonReleased(mUseRightStick ? "GrabRight" : "GrabLeft") && mIsGrabbing)
	{
		release();
	}

	if (InputManager::getInstance().isButtonPressed(mUseRightStick ? "PullRight" : "PullLeft") && mIsGrabbing)
	{
		pullUp();
	}

	if (InputManager::getInstance().isButtonReleased(mUseRightStick ? "PullRight" : "PullLeft"))
	{
		extend();
	}

	if (mIsPulling)
	{
		if (mCurLength > mPullLength)
		{
			mCurLength -= mPullSpeed * deltaTime;
			mJoint->SetMaxLength(mCurLength);
		}
	}
	else if (mCurLength < mNormalLength)
	{
		mCurLength += mPullSpeed * deltaTime;
		mJoint->SetMaxLength(mCurLength);
	}

	move(translation, mMoveSpeed * 100);

	mCurLength = PhysicsManager::UNRATIO* MathUtil::length(mBody->getPosition() - mGameObject.getPosition());

}

// for the normal, extended length
void HandMoveComponent::setJointLength(float length)
{
	mNormalLength = length;
}

// for the pull-up length
void HandMoveComponent::setJointPullLength(float length)
{
	mPullLength = length;
}

void HandMoveComponent::grab()
{
	auto distance = MathUtil::length(mGrabPosition - mGameObject.getPosition());

	if (distance <= 450.f)
	{
		mIsGrabbing = true;
		mRigidBody.getB2Body()->SetType(b2_staticBody);
		mGrabbedRock->get_component<IRockComponent>()->grabRock();
		mGrabbedRock->get_component<IRockComponent>()->setHand(std::make_shared<HandMoveComponent>(*this));
	}
	else
	{
		mCanGrab = false;
	}
}

void HandMoveComponent::release()
{
	auto hand = mRigidBody.getB2Body();
	hand->SetType(b2_dynamicBody);
	mIsGrabbing = false;
	mGrabbedRock->get_component<IRockComponent>()->releaseRock();

	// launch player if pulling up
	if (mIsPulling)
	{
		auto body = mJoint->GetBodyA();
		// from player to point between hands
		auto handsMidPoint = (mGameObject.getPosition() + mOtherHand->getGameObject().getPosition()) / 2.f;
		auto direction = (handsMidPoint - mBody->getPosition()) / MathUtil::length(handsMidPoint - mBody->getPosition());
		body->ApplyLinearImpulse(PhysicsManager::s2b(direction * 6000.f * PhysicsManager::RATIO), body->GetLocalCenter(), true);
		hand->ApplyLinearImpulse(PhysicsManager::s2b(direction * 1000.f * PhysicsManager::RATIO), hand->GetLocalCenter(), true);
	}
}

void HandMoveComponent::pullUp()
{
	mIsPulling = true;
	//mJoint->SetMaxLength(mPullLength);
}

void HandMoveComponent::extend()
{
	//mJoint->SetMaxLength(mNormalLength);
	mIsPulling = false;
}

void HandMoveComponent::move(sf::Vector2f direction, float speed)
{
	// if distance to body >= maxDistance and the other hand is not holding on, then no more force.
	// otherwise add force
	auto hand = mRigidBody.getB2Body();

	//if ((mCurLength > mPullLength - 1.5f && mIsPulling))
	//{
	//	speed = 0;
	//}
	//else if ((mCurLength > mNormalLength - 1.5f && !mIsPulling))
	//{
	//	speed = 0;
	//}

	if (mCurLength > mNormalLength - 1.5f && !mOtherHand->mIsGrabbing)
	{
		direction = (mBody->getPosition() - mGameObject.getPosition()) / MathUtil::length(mBody->getPosition() - mGameObject.getPosition());
		mBody->get_component<RigidBodyComponent>()->getB2Body()->ApplyForce(PhysicsManager::s2b(sf::Vector2f(0, 1) * speed / 5.f), b2Vec2(0, 0), true);
		//direction = sf::Vector2f(0, 1);
	}

	//if ((mCurLength >= mPullLength && mIsPulling) || (mCurLength >= mNormalLength && !mIsPulling))
	//{
	//	speed = 0;
	//}

	hand->ApplyForce(PhysicsManager::s2b(direction * speed), b2Vec2(0, 0), true);
}

void HandMoveComponent::onCollisionEnter(ColliderComponent& other)
{
	if (other.getGameObject().getTag() == "Grabbable")
	{
		mGrabbedRock = &other.getGameObject();
		mGrabPosition = other.getGameObject().getPosition();
		mCanGrab = true;
	}
}