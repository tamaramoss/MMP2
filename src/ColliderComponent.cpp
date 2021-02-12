/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "ColliderComponent.h"
#include "GameObject.h"

#include "EventBus.h"
#include "PhysicsComponentEvents.h"


ColliderComponent::ColliderComponent(GameObject& gameObject, RigidBodyComponent& body, b2FixtureDef& def) :
	IGameComponent(gameObject),
	mBody(body),
	mFixture(nullptr)
{
	def.userData = this;
	mFixture = mBody.getB2Body()->CreateFixture(&def);
}

void ColliderComponent::update(float deltaTime)
{
}

void ColliderComponent::registerOnCollisionFunction(const OnCollisionFunction& func)
{
	mOnCollisionFunctions.push_back(func);
}

void ColliderComponent::onCollision(ColliderComponent& collider)
{
	for (const auto& f : mOnCollisionFunctions)
		f(*this, collider);
}
