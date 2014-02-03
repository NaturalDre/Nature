#ifndef PHYSICS_H
#define PHYSICS_H

#include <Box2D\Box2D.h>
#include "TMR\TiledObject.h"
#include "PhysicsService.h"
const float SCALE(32.0f);

namespace Tiled
{
	class TiledObject;
}

namespace VE
{
	class CContactListener;
	class CCamera;
	class CPhysicsView;
	class CPhysics: public IPhysicsService
	{
	public:
		CPhysics(void);
		~CPhysics(void);

		inline void Simulate(void) const { m_world->Step(m_timeStep, m_velocityIters, m_positionIters); }
		inline void DrawDebugData(void) const { if (IsDebugDrawEnabled() && World()) World()->DrawDebugData(); }
		inline b2World* World(void) const { return m_world; }
		inline CContactListener* ContactListener(void) const { return m_contactListener; }
		inline bool IsDebugDrawEnabled(void) const { return m_drawDebugData; }
		inline void EnableDebugDraw(bool shouldDraw) { m_drawDebugData = shouldDraw; }
		static void Export(lua_State* L);
	private:
		float m_timeStep;
		size_t m_velocityIters, m_positionIters;
		bool m_drawDebugData;
		b2World* m_world;
		CPhysicsView* m_physicsView;
		CContactListener* m_contactListener;
	};


	/// Returns distance of position from the current camera's top left position.
	/// @param cam The camera that you want to use for the calculation.
	///	@param posPix The position whose distance from the camera you want.
	///
	///	\note The viewing area of the camera is considered the screen, so if you want
	///	to draw something that is at some position in the world and when you pass
	///	that position to this function and it returns (240, 530), then you draw it
	///	at (240, 530) on the screen. Consequently, if the X or Y position returned
	///	is larger than the camera's width/height(if those are also equal to the 
	///	game screen's width/height) then it is not visible on the screen.
	b2Vec2 GameToScreenPosPix(CCamera* cam, const b2Vec2& posPix);
	/// Converts meters to pixels.
	/// @param meters The meters to be converted.
	/// \return The converted meters to pixels value.
	inline float MtrToPix(float meters) { return (meters * SCALE); }
	/// Converts meters to pixels.
	/// @param vec Vector respresenting a 2D position in meters.
	///	\return The converted 2D vector's meter position as pixels.
	inline b2Vec2 MtrToPix(b2Vec2 vec) { vec *= SCALE; return vec; }
	inline float PixToMtr(float pixels) { return (pixels / SCALE); }
	inline b2Vec2 PixToMtr(b2Vec2 vec) { vec.x /= SCALE; vec.y /= SCALE; return vec; }

	/// Tiled uses the top left of a tile as it's origin. Box2D uses an object's center.
	/// This function converts a Tiled origin to a Box2D origin.
	inline b2Vec2 TiledToBoxCoords(b2Vec2& posPix, float widthPix, float heightPix) 
	{ 
		return PixToMtr(posPix) + b2Vec2(PixToMtr(widthPix) / 2.0f, PixToMtr(heightPix) / 2.0f); 
	}
	inline b2Vec2 TiledToBoxCoords(const Tiled::Object& obj)
	{
		return PixToMtr(b2Vec2(obj.X(), obj.Y())) + b2Vec2(PixToMtr(obj.Width()) / 2.0f, PixToMtr(obj.Height()) / 2.0f);
	}
	///	Tiled does not position it's objects using the center of the object like Box2D tends 
	///	to do. So we need a helper function to do that for us and return it in meters.
	b2Vec2 GetWorldCenterMtrs(const Tiled::Object& obj);

	inline float GetAngle(const b2Vec2& v1, const b2Vec2& v2)
	{
		return atan2(v2.y - v1.y, v2.x - v1.x);
	}
	/// @param angle Angle in radians to rotate by
	inline b2Vec2 Rotate(b2Vec2 v, float angle)
	{
		float xt = (v.x * cosf(angle)) - (v.y * sinf(angle));
		float yt = (v.y * cosf(angle)) + (v.x * sinf(angle));
		v.x = xt;
		v.y = yt;

		return v;
	}
}

#endif