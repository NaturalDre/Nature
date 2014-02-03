#ifndef ANIMATIONBASE_H
#define ANIMATIONBASE_H

#include "Animation.h"

namespace VE
{
	class IAnimationBase: public IAnimation
	{
	private:
		void NextFrame(void);

	protected:
		/// Called each time currentFrame changes. [Inside NextFrame() and SetCurrentFrame()]
		virtual void FrameChanged(void) { }
	public:
		virtual ~IAnimationBase(void);
		/*
		 * GetCurrentFrame()
		 *
		 * Returns: size_t
		 *		The current frame this animation is on.
		 *		E.G.: If an animation has 3 rows, 4 cols, and the
		 *		current frame is row 2, col 3, you're on the 7th frame.
		 */
		inline size_t GetCurrentFrame(void) const { return m_currentFrame; }
		// Do timing.
		void Logic(double dt);
		/// SetCurrentFrame()
		/// @param frame The frame you want the animation to be at currently.
		/// \note Does not reset how much time has passed since
		///	last frame changed. (aka Time last animated).
		inline void SetCurrentFrame(size_t frame) { m_currentFrame = frame; }
		/// How many rows does this animation have?
		inline size_t GetRows(void) const { return m_rows; }
		/// How many columns does this animation have?
		inline size_t GetCols(void) const { return m_cols; }
		/// How fast the animation is playing
		inline double GetFPS(void) const { return m_fps; }
		/// Change the framerate of the animation. TODO: Throw exception on <=0.
		inline void SetFPS(double fps) { if (fps >= 0) m_fps = fps; m_fps = 1; }
		/// Is the animation held? AKA the animation is frozen.
		inline bool IsAnimHeld(void) const { return m_holdAnimation; }
		/// Should the animation be held?
		inline void SetHoldAnimation(bool hold) { m_holdAnimation = hold; }
		/// The starting frame of the animation. When the end is reached it will start back at this frame.
		inline void SetBeginFrame(size_t frame) { if (frame > 0) { m_beginFrame = frame; FrameChanged(); }}
		/// The last frame of the animation. When this frame is reached the animation goes back to the start frame.
		inline void SetEndFrame(size_t frame) { if (frame > 0) m_endFrame = frame; }
		/// The first frame of the animation
		inline size_t GetBeginFrame(void) const { return m_beginFrame; }
		/// The last frame of the animation
		inline size_t GetEndFrame(void) const { return m_endFrame; }
		/* 
		 * Reset()
		 * 
		 * Reset the animation to the first frame. Time 'til next frame is reset.
		 * Sets hold animation to false.
		 */
		void Reset(void);
		// How much game time until this animation goes to the next frame?
		inline double TimeToNextFrame(void) const { return m_timeout; }

	protected:
		/*
		 * Constructor()
		 *
		 * Initializes to default parameters.
		 * 
		 * Default Params:
		 *		rows = 1;
		 *		cols = 1;
		 *		currentFrame = 1;
		 *		fps = 1.0;
		 *		timeout = 1.0
		 *		holdAnimation = false;
		 */
		IAnimationBase(void);
		/*
		 * Constructor()
		 *
		 * Params:
		 *		rows = How many rows the animation sheet has. Must be > 0
		 *		cols = How many columns the animation sheet has. Must be > 0
		 * Throws:
		 *		std::exception if row or col is zero.
		 */
		IAnimationBase(size_t rows, size_t cols, double fps);
		
		// Copy Constructor
		IAnimationBase(const IAnimationBase& rhs);
		// Assignment operator
		IAnimationBase& operator=(const IAnimationBase& rhs);
		/*	If true, when NextFrame() is called it will immedietly return 
		 *	without going to the next frame.
		 */
		bool m_holdAnimation;
		/*
		 * Each game update, the deltatime is subtracted from this. 
		 * When it's 0 or less it's time to call NextFrame().
		 */
		double m_timeout;
		// How many frames per second the animation should run at.
		double m_fps;
		// The current frame number we're on.
		size_t m_currentFrame;
		// How many rows the animation sheet has.
		size_t m_rows;
		// How many columns the animation sheet has.
		size_t m_cols;
		// The beginning frame of the animation.
		size_t m_beginFrame;
		// The end frame of the animation.
		size_t m_endFrame;
	};
}
#endif