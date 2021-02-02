#pragma once
#include "IGameComponent.h"
#include "RigidBodyComponent.h"

class PlayerBodyComponent : public IGameComponent
{
public:
	PlayerBodyComponent(GameObject& gameObject, RigidBodyComponent& rigidBody);

	bool init() override;
	void update(float deltaTime) override;

private:
	RigidBodyComponent& mRigidbody;
};