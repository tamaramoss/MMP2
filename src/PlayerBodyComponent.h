#pragma once
#include "IGameComponent.h"
#include "RigidBodyComponent.h"

class PlayerBodyComponent : public IGameComponent
{
public:
	PlayerBodyComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, float armLength);

	bool init() override;
	void update(float deltaTime) override;

private:
	float mArmLength = 300.f;
	RigidBodyComponent& mRigidbody;
};