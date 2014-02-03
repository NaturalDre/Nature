#ifndef RENDER_H
#define RENDER_H

#include "Camera.h"
#include "Process.h"
#include <vector>
//#include <Gwen\Controls\Canvas.h>

#include "RenderService.h"

namespace VE
{
	class IView;
	class CPhysics;
	class CBitmap;
	class CGameLevel;
	class CRenderComponent;
	class CRender: /*public IProcess,*/ public IRenderService
	{
	protected:
		void Think(double dt) { }

	public:
		CRender(void);
		~CRender(void);

		void Add(IView* view);
		void Remove(IView* view);

		void Add(CRenderComponent* rc);
		void Remove(CRenderComponent* rc);

		CCamera* Cam(void) const { return m_camera; }

		/*void SetUICanvas(GwenCanvas* canvas) { m_uiCanvas = canvas; }*/
		void Render(void);

		void SetCamera(CCamera* cam) { m_camera = cam; }
		CCamera* GetCamera(void) const { return m_camera; }
	private:
		std::vector<IView*> m_views;
		std::vector<CRenderComponent*> m_renderComponents;
		CGameLevel* m_gameLevel;
		//CGameMap* m_gameMap;
		CCamera* m_camera;
		// I only have a reference to the physics engine to draw the debug data for it.
		//CPhysics* m_physics;
		/*GwenCanvas* m_uiCanvas;*/
	};
	// Current width of the screen window.
	size_t GetDisplayWidth(void);
	// Current height of the screen window.
	size_t GetDisplayHeight(void);
}
#endif