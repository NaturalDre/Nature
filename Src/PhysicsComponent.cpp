#include "PhysicsComponent.h"
#include <Box2D\Box2D.h>

namespace VE
{
	CPhysicsComponent::CPhysicsComponent(CPhysics* physics, b2Body* body)
		: m_physics(physics)
		, m_body(body)
	{
		assert(physics != nullptr);
	}

	CPhysicsComponent::~CPhysicsComponent(void)
	{

	}
}