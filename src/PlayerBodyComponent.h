/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once
#include "IGameComponent.h"
#include "RigidBodyComponent.h"
#include "HandMoveComponent.h"


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
	void onCollision(ColliderComponent& other);
	void addHand(std::shared_ptr<HandMoveComponent> hand);

private:
	RigidBodyComponent& mRigidbody;
	bool mLevelWin = false;
	bool mPlayerDead = false;
	std::vector< std::shared_ptr<HandMoveComponent> > mHands;
};