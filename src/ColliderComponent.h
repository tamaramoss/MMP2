/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once
#include <functional>
#include <list>
#include "IGameComponent.h"
#include "RigidBodyComponent.h"
#include <Box2D/Box2D.h>

class ColliderComponent : public IGameComponent
{
public:
	/// \param collider1 the collider for which OnCollision was called.
	/// \param collider2 the collider with which the collision happened.
	using OnCollisionFunction = std::function<void (ColliderComponent&, ColliderComponent&)>;
	
	/// \param body the rigid body the collider is attached to.
	/// \param rect the size of the AABB in pixel
	ColliderComponent(GameObject& gameObject, RigidBodyComponent& body, b2FixtureDef& def);
	
	bool init() override { return true; }
	virtual void update(float fDeltaTime) override;
	
	/// Add delegate function to be executed when collision is detected.
	/// Signature: void func(ColliderComponent&)
	/// 
	void registerOnCollisionFunction(const OnCollisionFunction& func);
	/// Method called when collision occured. Method calls all subscribed
	/// OnCollisionFunctions
	///
	/// \param collider collision occured with this collider
	void onCollision(ColliderComponent& collider);
	
	RigidBodyComponent& getBody() const { return mBody; }
	
private:
	RigidBodyComponent& mBody;
	b2Fixture* mFixture;
	std::list<OnCollisionFunction> mOnCollisionFunctions;
};
