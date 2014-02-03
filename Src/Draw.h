#pragma once
#include <Bitmap.hpp>
#include <Box2D\Box2D.h>

namespace VE
{
	bool IsDrawable(const alx::Bitmap& bitmap, b2Vec2 bitmapPos, b2Vec2 bitmapCPos = b2Vec2(0,0));

	///// Draw a bitmap to the screen
	///// @param cpos the position the bitmap should be rotated around
	//bool DrawBitmap(const alx::Bitmap& bitmap, b2Vec2 dPos, b2Vec2 cPos);
	void DrawBitmap(const alx::Bitmap& bitmap, b2Vec2 dPos, b2Vec2 cPos, float angle = 0, int flags = 0);
	void DrawBitmap(const alx::Bitmap& bitmap, b2Vec2 dpos, int flags = 0);
}