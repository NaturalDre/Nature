#ifndef CAMERA_H
#define CAMERA_H

#include "Physics.h"

namespace VE
{
	class CPlayer;
	class CCamera
	{
		// IGameObject needs access to UnWatch()
		friend CPlayer;
	protected:
		/* 
		* UnWatch()
		*
		* Tell the camera not to follow an object if it's
		* pointer is the one passed.
		*
		* Params:
		*	IGameObject* - The gameobject to stop watching.
		*
		* Notes:
		*	I need this function because I need to ensure that whenever
		*	a game object is destroyed that, if the camera is following it,
		*	the camera doesn't try to use an invalid pointer. This function
		*	is meant to only be called by IGameObject's destructor.
		*/
		void UnWatch(CPlayer* player);

	public:
		// Pass an object to follow. Null pointer means camera focuses on (0,0) in the world.
		CCamera(CPlayer* player = nullptr, size_t width = 800, size_t height = 600);
		~CCamera(void);

		// 2D Vector of the camera's center/focus position in pixels.
		b2Vec2 PosPix(void) const;
		// 2D vector of the camera' center/focus position in meters.
		b2Vec2 PosMtr(void) const;
		/*
		* TopLeftPosPix()
		*
		* Gets the gameworld position of the topleft of the camera view.
		*
		* Notes:
		*	The top left of the camera corresponds to the to the top left
		*	of the computer screen, which should be (0,0). Everything below
		*	(0,0) is not viewable. Everything past (0,0) + b2Vec2(camWidth,camHeight)
		*	is also not viewable.
		*/
		b2Vec2 TopLeftPosPix(void) const 
		{ 
			b2Vec2 tl = MtrToPix(PosMtr());
			tl.x -= m_width / 2.0f;
			tl.y -= m_height / 2.0f;
			return tl;
		} 
		/// Set the player for the camera to follow.
		/// @param player The player to follow.
		/*inline */void Watch(CPlayer* player) { m_player = player; }
		/// Set the width of the camera
		/// @param width Pixels.
		void SetWidth(size_t width) { if (width) m_width = width; }
		/// Set the height of the camera.
		/// @param Pixels.
		void SetHeight(size_t height) { if (height) m_height = height; }
		/// Width of the camera.
		/// \return Pixels.
		inline size_t Width(void) const { return m_width; }
		/// Height of the camera.
		/// \return Pixels.
		inline size_t Height(void) const { return m_height; }

	private:
		// The object the camera is following. Null means none and focus on (0,0).
		CPlayer* m_player;
		// The width of the camera. Pix.
		size_t m_width;
		// The height of the camera. Pix.
		size_t m_height;
	};
}
#endif