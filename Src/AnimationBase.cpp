#include <allegro5\allegro5.h>
#include "AnimationBase.h"

namespace VE
{
	IAnimationBase::IAnimationBase(void)
		: m_holdAnimation(false)
		, m_timeout(1.0f)
		, m_fps(1.0f)
		, m_currentFrame(1)
		, m_rows(1)
		, m_cols(1)
		, m_beginFrame(1)
		, m_endFrame(1)
	{

	}

	IAnimationBase::IAnimationBase(size_t rows, size_t cols, double fps)
		: m_holdAnimation(false)
		, m_timeout(0.0)
		, m_fps(fps)
		, m_currentFrame(1)
		, m_rows(rows)
		, m_cols(cols)
		, m_beginFrame(1)
		, m_endFrame(1)

	{
		if (m_fps <= 0)
			m_fps = 1.0;
		if (m_rows < 1)
			m_rows = 1;
		if (m_cols < 1)
			m_cols = 1;

		m_endFrame = m_rows * m_cols;
		m_timeout = 1.0 / m_fps;
	}

	IAnimationBase::IAnimationBase(const IAnimationBase& rhs)
		: m_holdAnimation(rhs.m_holdAnimation)
		, m_timeout(rhs.m_timeout)
		, m_fps(rhs.m_fps)
		, m_currentFrame(rhs.m_currentFrame)
		, m_rows(rhs.m_rows)
		, m_cols(rhs.m_cols)
		, m_beginFrame(rhs.m_beginFrame)
		, m_endFrame(rhs.m_endFrame)
	{
		//GetObjMgr().Add(this);
	}

	IAnimationBase& IAnimationBase::operator=(const IAnimationBase& rhs)
	{
		if (this == &rhs)
			return *this;

		m_holdAnimation = rhs.m_holdAnimation;
		m_timeout = rhs.m_timeout;
		m_fps = rhs.m_fps;
		m_currentFrame = rhs.m_currentFrame;
		m_rows = rhs.m_rows;
		m_cols = rhs.m_cols;
		m_beginFrame = rhs.m_beginFrame;
		m_endFrame = rhs.m_endFrame;

		return *this;
	}	
	
	IAnimationBase::~IAnimationBase(void)
	{
		//GetObjMgr().Remove(this);
	}

	void IAnimationBase::NextFrame(void)
	{
		if (m_holdAnimation)
			return;

		++m_currentFrame;
		// Are we at the end of the animation?
		if ((m_currentFrame > (m_rows * m_cols)) || m_currentFrame > m_endFrame)
			m_currentFrame = m_beginFrame;	// If so, start over
		FrameChanged();
	}

	void IAnimationBase::Logic(double dt)
	{
		m_timeout -= dt;
		if (m_timeout <= 0)
		{
			m_timeout = 1.0f / m_fps;
			NextFrame();
		}
	}

	void IAnimationBase::Reset(void)
	{
		m_currentFrame = m_beginFrame;;
		FrameChanged();
		m_timeout = 1.0f / m_fps;
		m_holdAnimation = false;
	}
}