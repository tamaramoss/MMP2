#include "stdafx.h"

#include "BoxColliderComponent.h"
#include "GameObject.h"

#include "EventBus.h"
#include "PhysicsComponentEvents.h"

BoxColliderComponent::BoxColliderComponent(GameObject& game_object,
	RigidBodyComponent& body, const FloatRect& AABB)
	: IComponent(game_object)
	, m_body(body)
	, m_AABB(AABB)
	, m_transformed_aabb(AABB)
{
	// send event that collider was created (e.g., to notify physics)
	EventBus::get_instance().fire_event(std::make_shared<BoxColliderCreateEvent>(*this));
}

void
BoxColliderComponent::update(float delta_time)
{
	const auto& transform = m_game_object.getTransform();
	m_transformed_aabb = transform.transformRect(m_AABB);
}

void
BoxColliderComponent::register_on_collision_function(const on_collision_function& func)
{
	m_on_collision_functions.push_back(func);
}

void
BoxColliderComponent::on_collision(BoxColliderComponent& collider)
{
	for (const auto& f : m_on_collision_functions)
		f(*this, collider);
}
