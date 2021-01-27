#pragma once

#include "IGameComponent.h"

#include "RigidBodyComponent.h"

class PlayerMoveComponent : public IGameComponent
{
public:
	using ptr = std::shared_ptr<PlayerMoveComponent>;

	PlayerMoveComponent(GameObject& gameObject, RigidBodyComponent& rigidBody,
	                    int playerIndex);

	bool init() override;
	void update(float deltaTime) override;

private:
	int m_playerIndex;

	RigidBodyComponent& m_rigidBody;
};
