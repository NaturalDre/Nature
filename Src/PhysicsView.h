#pragma once

#include "View.h"
#include "DebugDraw.h"

namespace VE
{
	class CPhysics;
	/// This class is for Debug purposes only. It draws debug data from Box2D
	class CPhysicsView: public IView
	{
	public:
		CPhysicsView(CPhysics*);
		~CPhysicsView(void);

	protected:
		void Draw(void);
		CPhysics* GetPhysics(void) const { return m_physics; }
	private:
		CPhysics* m_physics;
		DebugDraw m_debugDraw;
	};
}