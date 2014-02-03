#include "Physics.h"
#include "ContactListener.h"
#include "DebugDraw.h"
#include "Camera.h"
#include <lua.hpp>
#include "PhysicsView.h"

namespace VE
{
	CPhysics::CPhysics(void)
		: m_world(nullptr)
		, m_timeStep(1.0f / 60.0f)
		, m_velocityIters(6)
		, m_positionIters(6)
		, m_drawDebugData(false)
	{
		m_world = new b2World(b2Vec2(0, 0));
		m_physicsView = new CPhysicsView(this);
		m_contactListener = new CContactListener(this);
		m_world->SetContactListener(m_contactListener);
	}

	CPhysics::~CPhysics(void)
	{
		delete m_world;
		m_world = nullptr;

		delete m_physicsView;
		m_physicsView = nullptr;

		delete m_contactListener;
		m_contactListener = nullptr;
	}

	b2Vec2 GameToScreenPosPix(CCamera* cam, const b2Vec2& posPix)
	{ 
		if (cam)
			return posPix - cam->TopLeftPosPix();
		return posPix;
	}
}