#pragma once

#include "Component.h"
#include <Box2D\Box2D.h>

namespace VE
{
	class CPhysics;
	class CPhysicsComponent: public IComponent
	{
	public:
		CPhysicsComponent(CPhysics* physics, b2Body* body);
		virtual ~CPhysicsComponent(void);

		virtual void BeginContact(b2Contact* contact) { }
		virtual void EndContact(b2Contact* contact) { }

		inline b2Body* GetRaw(void) const { return m_body; }
		inline b2Vec2 GetPosMtrs(void) const { return m_body->GetPosition(); }
		inline float GetAngle(void) const { return m_body->GetAngle(); }

	private:
		b2Body* m_body;
		CPhysics* m_physics;
	};
}