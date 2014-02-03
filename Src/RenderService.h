#pragma once

namespace VE
{
	class IView;
	class CCamera;
	class IRenderService
	{
	public:
		virtual ~IRenderService(void) { }
		/// \note Let every IView know it is time to render.
		virtual void Render(void) = 0;
		virtual void Add(IView* view) = 0;
		virtual void Remove(IView* view) = 0;

		virtual CCamera* GetCamera(void) const = 0;
		virtual void SetCamera(CCamera* cam) = 0;
	};
}