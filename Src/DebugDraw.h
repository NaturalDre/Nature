#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <iostream>
#include <Box2D/Box2D.h>
#include <Box2D\Dynamics\b2WorldCallbacks.h>

#include <cstdio>
#include <cstdarg>

#include <cstring>
//#include <vengine\Render\Render.h>

using namespace std;

namespace VE
{
	class CCamera;
}

struct b2AABB;
class DebugDraw : public b2Draw
{
public:
	DebugDraw(VE::CCamera* cam)
		: m_cam(cam)
	{
	uint32 flags = 0;
	flags = e_shapeBit | e_jointBit /*| e_aabbBit*/ | e_pairBit | e_centerOfMassBit;
	//flags += 1			* b2Draw::e_shapeBit;
	//flags += 1			* b2Draw::e_jointBit;
	////flags += 1			* b2Draw::e_aabbBit;
	//flags += 1			* b2Draw::e_pairBit;
	//flags += 1			* b2Draw::e_centerOfMassBit;
	this->SetFlags(flags);
	}
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    void DrawString(int x, int y, const char* string, ...); 
    void DrawAABB(b2AABB* aabb, const b2Color& color);

private:
	VE::CCamera* m_cam;
};

#endif