#include "PhysicsView.h"
#include "Locator.h"
#include "RenderService.h"
#include "Physics.h"

namespace VE
{
	CPhysicsView::CPhysicsView(CPhysics* physics)
		: IView(10)
		, m_physics(physics)
		, m_debugDraw(CLocator::GetRenderer()->GetCamera())
	{
		assert(physics != nullptr);
		GetPhysics()->World()->SetDebugDraw(&m_debugDraw);
	}

	CPhysicsView::~CPhysicsView(void)
	{

	}

	void CPhysicsView::Draw(void)
	{
		GetPhysics()->World()->DrawDebugData();
	}
}