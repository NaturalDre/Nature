#pragma once

#include <Bitmap.hpp>

namespace VE
{
	class CRender;
	class CRenderComponent
	{
	public:
		enum DrawType { 
			Basic, Region, Tinted, 
			TintedRegion, Rotated, 
			TintedRotated, ScaledRotated, 
			Scalted, TintedScaled };

		CRenderComponent(CRender* renderer);
		~CRenderComponent(void);

		const alx::Bitmap& GetBitmap(void) const { return m_bitmap; }
		float GetDX(void) const { return m_dx; }
		float GetDY(void) const { return m_dy; }
		int GetDrawLayer(void) const { return m_drawLayer; }
		int GetFlags(void) const { return m_flags; }

		void SetBitmap(const alx::Bitmap& bitmap) { m_bitmap = bitmap; }
		void SetDX(float dx) { m_dx = dx; }
		void SetDY(float dy) { m_dy = dy; }
		inline DrawType GetDrawType(void) const { return m_drawType; }



	protected:
		/// This should only be called once for derived typed in the constructor.
		void SetDrawType(DrawType type) { m_drawType = type; }

	private:
		CRender* m_renderer;
		DrawType m_drawType;
		alx::Bitmap m_bitmap;
		//ALLEGRO_COLOR m_color;
		float m_dx;
		float m_dy;
		int m_drawLayer;
		int m_flags;
	};

	class CRenderComponentRotated: public CRenderComponent
	{
	public:
		CRenderComponentRotated(CRender* renderer)
			: CRenderComponent(renderer)
			, m_cx(0), m_cy(0)
			, m_angle(0.0f) 
		{ SetDrawType(Rotated); }

		float GetCX(void) { return m_cx; }
		float GetCY(void) const { return m_cy; }
		float GetAngle(void) const { return m_angle; }

		void SetCX(float cx) { m_cx = cx; }
		void SetCY(float cy) { m_cy = cy; }
		void SetAngle(float cy) { m_angle; }

	private:
		float m_cx;
		float m_cy;
		float m_angle;
	};
}