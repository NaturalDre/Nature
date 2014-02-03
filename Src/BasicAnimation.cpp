#include "BasicAnimation.h"
#include <allegro5\allegro5.h>
#include "Utility.h"

#define DEFAULTFRAMEW 1
#define DEFAULTFRAMEH 1

namespace VE
{
	CBasicAnimation::CBasicAnimation(void)
		: IAnimationBase()
		, m_animationSheet()
		, m_frame()
		, m_frameW(DEFAULTFRAMEW)
		, m_frameH(DEFAULTFRAMEH)
		, m_framePosX(0)
		, m_framePosY(0)
	{

	}

	CBasicAnimation::CBasicAnimation(const alx::Bitmap& sheet, size_t rows, size_t cols, double fps)
		: IAnimationBase(rows, cols, fps)
		, m_animationSheet(sheet)
		, m_frame()
		, m_frameW(DEFAULTFRAMEW)
		, m_frameH(DEFAULTFRAMEH)
		, m_framePosX(0)
		, m_framePosY(0)
	{
		assert(m_animationSheet.get() != nullptr);
		assert(rows > 0);
		assert(cols > 0);
		assert(fps > 0.0);

		if (!m_animationSheet || !GetRows() || !GetCols() || (GetFPS() <= 0))
			abort();

		if (m_animationSheet)
		{
			m_frameW = m_animationSheet.getWidth() / GetCols();
			m_frameH = m_animationSheet.getHeight() / GetRows();
			FrameChanged();
		}
	}

	CBasicAnimation::CBasicAnimation(const CBasicAnimation& rhs)
		: IAnimationBase(rhs)
		, m_frame()
		, m_animationSheet()
		, m_frameW(0)
		, m_frameH(0)
		, m_framePosX(0)
		, m_framePosY(0)
	{
		*this = rhs;
	}

	CBasicAnimation::CBasicAnimation(CBasicAnimation&& rhs)
		: IAnimationBase(rhs)
		, m_frame()
		, m_animationSheet()
		, m_frameW(1)
		, m_frameH(1)
		, m_framePosX(0)
		, m_framePosY(0)
	{
		*this = std::move(rhs);
	}

	CBasicAnimation& CBasicAnimation::operator=(const CBasicAnimation& rhs)
	{
		if (this == &rhs)
			return *this;

		IAnimationBase::operator=(rhs);

		if (m_frame)
			m_frame.reset();

		if (m_animationSheet)
			m_animationSheet.reset(); // .reset because it's a smart pointer. We don't want to call ->Unload() incase something needs it.
		
		m_animationSheet = rhs.m_animationSheet;
		m_frameW = rhs.m_frameW;
		m_frameH = rhs.m_frameH;
		m_framePosX = rhs.m_framePosX;
		m_framePosY = rhs.m_framePosY;

		// Force a call to FrameChanged() to generate the image for the current frame.
		FrameChanged();
		// Note:	I did this 'cause I don't know if a clone of the sub-bitmap(rhs.m_frame) is 
		//			still a sub-bitmap of the original bitmap(rhs.m_animationSheet), a 
		//			sub-bitmap of the sub-bitmap(rhs.m_frame), or is now its own bitmap.
		return *this;
	}

	CBasicAnimation& CBasicAnimation::operator=(CBasicAnimation&& rhs)
	{
		if (this == &rhs)
			return *this;

		IAnimationBase::operator=(std::move(rhs));

		if (m_frame)
			m_frame.reset();

		if (m_animationSheet.get())
			m_animationSheet.reset();

		if (rhs.m_animationSheet.get())
			m_animationSheet = std::move(rhs.m_animationSheet);

		m_frameW = rhs.m_frameW;
		m_frameH = rhs.m_frameH;
		m_framePosX = rhs.m_framePosX;
		m_framePosY = rhs.m_framePosY;

		// Force a call to FrameChanged() to generate the image for the current frame.
		FrameChanged();
		// Note:	I did this 'cause I don't know if a clone of the sub-bitmap(rhs.m_frame) is 
		//			still a sub-bitmap of the original bitmap(rhs.m_animationSheet), a 
		//			sub-bitmap of the sub-bitmap(rhs.m_frame), or is now its own bitmap.
		return *this;
	}

	CBasicAnimation::~CBasicAnimation()
	{
		// We need to free this before m_animationSheet because it's a subbitmap of m_animationSheet.
		if (m_frame)
			m_frame.reset();
		m_animationSheet.reset();
	}

	void CBasicAnimation::FrameChanged(void)
	{
		if (!m_animationSheet)
			return;

		const ColRow cr = GetColRow(GetCols(), GetCurrentFrame());
		const size_t x((cr.col - 1) * m_frameW);
		const size_t y((cr.row - 1) * m_frameH);

		if (m_frame)
			m_frame.reset();
		// Create subbitmap
		m_frame = alx::Bitmap(m_animationSheet, x, y, m_frameW, m_frameH);
		
		m_framePosX = x;
		m_framePosY = y;
	}

	void CBasicAnimation::SetAlpha(size_t r, size_t g, size_t b)
	{
		m_animationSheet.convertMaskToAlpha(al_map_rgb(r, g, b));
	}
}