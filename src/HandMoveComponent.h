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

	// gameplay
	void grab();
	void release();
	void pullUp();
	void extend();
	void move(sf::Vector2f direction, float speed);
	void onCollisionEnter(ColliderComponent& other);

private:
	// references
	RigidBodyComponent& mRigidBody;
	std::shared_ptr<HandMoveComponent> mOtherHand;
	std::shared_ptr<GameObject> mBody;

	// gameplay params
	sf::Vector2f mGrabPosition;
	float mMoveSpeed = 3500.f;
	float mNormalLength = 40.f;
	float mPullLength = 15.f;

	bool mCanGrab = false;
	bool mIsGrabbing = false;

	// control params
	int mPlayerIndex = 0;
	bool mUseRightStick = false;
};