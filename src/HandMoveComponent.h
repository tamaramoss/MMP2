/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once
#include "IGameComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include <SFML\System\Vector2.hpp>

class HandMoveComponent : public IGameComponent
{
public:
	HandMoveComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, int playerIndex, bool useRightStick);
	
	bool init() override;
	void update(float deltaTime) override;

	// settings
	void setJointLength(float length);
	void setJointPullLength(float length);
	void setOtherHandReference(std::shared_ptr<HandMoveComponent> otherHand) { mOtherHand = otherHand; }
	void setBodyReference(std::shared_ptr<GameObject> body) { mBody = body; }
	void setJoint(b2RopeJoint* joint) { mJoint = joint; }

	// gameplay
	void grab();
	void release();
	void pullUp();
	void extend();
	void move(sf::Vector2f direction, float speed);
	void onCollisionEnter(ColliderComponent& other);

	// access
	float getMaxLength() { return mNormalLength; }
	void setReleaseFlag(bool flag);

private:
	// references
	RigidBodyComponent& mRigidBody;
	std::shared_ptr<HandMoveComponent> mOtherHand;
	std::shared_ptr<GameObject> mBody;
	b2RopeJoint* mJoint;

	// gameplay params
	GameObject* mGrabbedRock;
	sf::Vector2f mGrabPosition;
	float mMoveSpeed = 1000.f;
	float mNormalLength = 45.f;
	float mPullLength = 20.f;
	float mCurLength = 40.f;
	float mPullSpeed = 23000.f;
	float mMaxDistanceToOtherHand = 2300.f;

	bool mCanGrab = false;
	bool mIsGrabbing = false;
	bool mIsPulling = false;
	bool mReleaseFlag = false;

	float mReleaseTimer = 0;
	float mNoGrabTime = 1.1f;

	// control params
	int mPlayerIndex = 0;
	bool mUseRightStick = false;
};