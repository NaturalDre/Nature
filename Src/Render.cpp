#include "Render.h"
#include "View.h"
#include "GameMap.h"
#include "Bitmap.hpp"
#include <algorithm>
#include <allegro5\allegro5.h>
#include "RenderComponent.h"

namespace VE
{
	CRender::CRender(void)
		: /*IProcess(nullptr)*/
/*		,*/ m_gameLevel(nullptr)
		, m_camera(nullptr)
		//, m_uiCanvas(nullptr)
	{
		m_camera = new CCamera(nullptr, GetDisplayWidth(), GetDisplayHeight());
	}

	CRender::~CRender(void)
	{
		delete m_camera;
		SetCamera(nullptr);
	}

	void CRender::Add(IView* view)
	{
		if (view == nullptr)
			return;

		auto iter = std::find(m_views.begin(), m_views.end(), view);
		if (iter == m_views.end())
			m_views.push_back(view);
	}

	void CRender::Remove(IView* view)
	{
		if (view == nullptr)
			return;

		auto iter = std::find(m_views.begin(), m_views.end(), view);
		if (iter != m_views.end())
			m_views.erase(iter);
	}

	void CRender::Add(CRenderComponent* view)
	{
		if (view == nullptr)
			return;

		auto iter = std::find(m_renderComponents.begin(), m_renderComponents.end(), view);
		if (iter == m_renderComponents.end())
			m_renderComponents.push_back(view);
	}

	void CRender::Remove(CRenderComponent* view)
	{
		if (view == nullptr)
			return;

		auto iter = std::find(m_renderComponents.begin(), m_renderComponents.end(), view);
		if (iter != m_renderComponents.end())
			m_renderComponents.erase(iter);
	}



	void CRender::Render(void)
	{
		if (!m_camera)
			return;

		std::sort(m_views.begin(), m_views.end(), [](IView* lhs, IView* rhs)
		{
			return lhs->DrawOrder() < rhs->DrawOrder();
		});

		for (auto iter = m_views.begin(); iter != m_views.end(); ++iter)
			(*iter)->Draw();

		for (auto iter = m_renderComponents.begin(); iter != m_renderComponents.end(); ++iter)
		{
			if ((*iter)->GetBitmap().get() != nullptr && (*iter)->GetDrawType() == CRenderComponent::Rotated)
			{
				CRenderComponentRotated* rc = static_cast<CRenderComponentRotated*>((*iter));
				al_draw_rotated_bitmap(
					rc->GetBitmap().get()
					, rc->GetCX()
					, rc->GetCY()
					, rc->GetDX()
					, rc->GetDY()
					, rc->GetAngle()
					, rc->GetFlags());

			}
		}

		//if (m_uiCanvas)
		//	m_uiCanvas->RenderCanvas();
		al_flip_display();
		al_clear_to_color(al_map_rgb(0,0,0));

	}

	size_t GetDisplayWidth(void)
	{
		return al_get_display_width(al_get_current_display());
	}

	size_t GetDisplayHeight(void)
	{
		return al_get_display_height(al_get_current_display());
	}
}