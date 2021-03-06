/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "HandMoveComponent.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <iostream>

#include "AnimationComponent.h"
#include "VectorAlgebra2D.h"
#include "PlayerBodyComponent.h"
#include "IRockComponent.h"
#include "SoundComponent.h"


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
	sf::Vector2f translation;
	translation.x = InputManager::getInstance().getAxisPosition("JoystickX", mPlayerIndex);
	translation.y = InputManager::getInstance().getAxisPosition("JoystickY", mPlayerIndex);

	if ((translation.x == 0 && translation.y == 0))
	{
		mRigidBody.getB2Body()->SetGravityScale(850);
	}
	else
	{
		mRigidBody.getB2Body()->SetGravityScale(0);
	}

	// grabbing
	if (InputManager::getInstance().isButtonDown("Grab", mPlayerIndex) && mCanGrab && !mIsGrabbing && mReleaseTimer <= 0)
	{
		grab();
	}
	else if(mReleaseTimer > 0)
	{
		mReleaseTimer -= deltaTime;
	}


	if ((InputManager::getInstance().isButtonReleased("Grab", mPlayerIndex) && mIsGrabbing) || mReleaseFlag)
	{
		release();
	}

	if (InputManager::getInstance().isButtonPressed("Pull", mPlayerIndex) && mIsGrabbing)
	{
		pullUp();
	}

	if (InputManager::getInstance().isButtonReleased("Pull", mPlayerIndex))
	{
		extend();
	}

	if (mIsPulling)
	{
		if (mCurLength > mPullLength)
		{
			mCurLength -= deltaTime * mPullSpeed;
			//mJoint->SetMaxLength(mCurLength);
			auto direction = mGameObject.getPosition() - mBody->getPosition();
			direction = direction / MathUtil::length(direction);
			mBody->get_component<RigidBodyComponent>()->getB2Body()->ApplyForce(PhysicsManager::s2b(direction * mPullSpeed * PhysicsManager::RATIO), b2Vec2(0,0), true);
		}
	}
	else
	{
		if (mCurLength < mNormalLength)
		{
			mCurLength += deltaTime * mPullSpeed;
			//mJoint->SetMaxLength(mCurLength);

		}
	}

	if(translation.x != 0 || translation.y != 0)
		move(translation, mMoveSpeed * PhysicsManager::RATIO);

	mCurLength = PhysicsManager::UNRATIO * MathUtil::length(mBody->getPosition() - mGameObject.getPosition());

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
		mGameObject.get_component<AnimationComponent>()->setAnimation("ToClose");
		mGameObject.get_component<SoundComponent>()->setSound("Grab");
		
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
	if (!mIsGrabbing)
		return;

	mReleaseFlag = false;
	mReleaseTimer = mNoGrabTime;

	auto hand = mRigidBody.getB2Body();
	hand->SetType(b2_dynamicBody);
	mIsGrabbing = false;
	mGameObject.get_component<AnimationComponent>()->setAnimation("ToOpen");

	
	if(mGrabbedRock != nullptr)
		mGrabbedRock->get_component<IRockComponent>()->releaseRock();
	mGrabbedRock = nullptr;
	mGrabPosition = sf::Vector2f(-1000000,-1000000);

	// launch player if pulling up
	if (mIsPulling)
	{
		mGameObject.get_component<SoundComponent>()->setSound("Pull");

		auto body = mBody->get_component<RigidBodyComponent>()->getB2Body();
		// from player to point between hands
		auto handsMidPoint = (mGameObject.getPosition() + mOtherHand->getGameObject().getPosition()) / 2.f;

		// if player is lower than hands, pull up, otherwise don't do anything
		auto direction = (handsMidPoint - mBody->getPosition()) / MathUtil::length(handsMidPoint - mBody->getPosition());
		direction = sf::Vector2f(0, -1);

		if (/*mBody->getPosition().y > handsMidPoint.y*/true)
		{
			body->ApplyLinearImpulse(PhysicsManager::s2b(direction * 800000.f * PhysicsManager::RATIO), body->GetLocalCenter(), true);
			hand->ApplyLinearImpulse(PhysicsManager::s2b(direction * 800000.f * PhysicsManager::RATIO), hand->GetLocalCenter(), true);

		}	
	}
}

void HandMoveComponent::pullUp()
{
	mIsPulling = true;
	//mJoint->SetMaxLength(mPullLength);
	mBody->get_component<AnimationComponent>()->setAnimation("Jump");

	//TODO make body animation back to "Default" when mIsPulling wieder auf falsch gesetzt wird
}

void HandMoveComponent::extend()
{
	mIsPulling = false;
	mBody->get_component<AnimationComponent>()->setAnimation("Default");

	//mJoint->SetMaxLength(mNormalLength);

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
	if(mOtherHand->mIsGrabbing)
	{

		auto directionPlayerToHand = (mGameObject.getPosition() - mBody->getPosition()) / MathUtil::length(mBody->getPosition() - mGameObject.getPosition());
		mBody->get_component<RigidBodyComponent>()->getB2Body()->ApplyForce(PhysicsManager::s2b(directionPlayerToHand * speed * 5.f), b2Vec2(0, 0), true);
	}

	// omg wth
	if (mCurLength > mNormalLength - 1.5f && !mOtherHand->mIsGrabbing)
	{
		//direction = (mBody->getPosition() - mGameObject.getPosition()) / MathUtil::length(mBody->getPosition() - mGameObject.getPosition());
		//mBody->get_component<RigidBodyComponent>()->getB2Body()->ApplyForce(PhysicsManager::s2b(sf::Vector2f(0, 1) * speed * 5.f), b2Vec2(0, 0), true);
		//direction = sf::Vector2f(0, 1);
	}

	//if ((mCurLength >= mPullLength && mIsPulling) || (mCurLength >= mNormalLength && !mIsPulling))
	//{
	//	speed = 0;
	//}
	//auto distanceToOther = MathUtil::length(mGameObject.getPosition() - mOtherHand->getGameObject().getPosition());


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

void HandMoveComponent::setReleaseFlag(bool flag)
{
	mReleaseFlag = flag;
}