#pragma once
#include "IGameComponent.h"
#include "RigidBodyComponent.h"

class PlayerBodyComponent : public IGameComponent
{
public:
	PlayerBodyComponent(GameObject& gameObject, RigidBodyComponent& rigidBody);

	bool init() override;
	void update(float deltaTime) override;

	void playerDied() { mPlayerDead = true; }
	void playerWon() { mLevelWin = true; }
	bool isPlayerDead() { return mPlayerDead; }
	bool isLevelWon() { return mLevelWin; }

private:
	RigidBodyComponent& mRigidbody;
	bool mLevelWin = false;
	bool mPlayerDead = false;
};