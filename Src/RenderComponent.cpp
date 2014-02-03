#include "RenderComponent.h"
#include "Render.h"

namespace VE
{
	CRenderComponent::CRenderComponent(CRender* renderer)
		: m_renderer(renderer)
		, m_drawType(Basic)
		, m_bitmap()
		//, m_color()
		, m_dx(0)
		, m_dy(0)
		, m_drawLayer(0)
		, m_flags(0)
	{
		assert(renderer != nullptr);
		if (m_renderer)
			m_renderer->Add(this);
	}

	CRenderComponent::~CRenderComponent(void)
	{
		if (m_renderer)
			m_renderer->Remove(this);
	}
}