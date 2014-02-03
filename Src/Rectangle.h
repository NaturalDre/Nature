#pragma once

namespace VE
{
	class CRectangle
	{
	public:
		CRectangle(void)
			: m_x(0)
			, m_y(0)
			, m_width(0)
			, m_length(0)
		{}

		inline float GetX(void) const { return m_x; }
		inline float GetY(void) const { return m_y; }
		inline void SetX(float x) { m_x = x; }
		inline void SetY(float y) { m_y = y; }

		inline float GetWidth(void) const { return m_width; }
		inline float GetLength(void) const { return m_length; }
		inline void SetWidth(float w) { m_width = w; }
		inline void SetHeight(float h) { m_length = h; }

		float GetArea(void) { return GetWidth() * GetLength(); }
		float GetPerimeter(void) { return (2 * GetWidth()) + (2 * GetLength()); }

	private:
		float m_x;
		float m_y;
		float m_width;
		float m_length;
	};	
}