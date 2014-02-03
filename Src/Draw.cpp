#include "Draw.h"
#include "Locator.h"
#include "Camera.h"
#include "Render.h"
#include "Physics.h"

//bool VE::DrawBitmap(const alx::Bitmap& bitmap, b2Vec2 dPos, b2Vec2 cPos)
//{
//	if (!bitmap)
//		return false;
//
//	VE::CCamera* cam = VE::CLocator::GetRenderer()->GetCamera();
//
//	b2Vec2 camTopL = cam->TopLeftPosPix();
//	// Copy the vector then add it's with and height to get its bottom right
//	b2Vec2 camBotR(camTopL);
//	camBotR.x += cam->Width();
//	camBotR.y += cam->Height();
//
//	// Convert to pix
//	dPos = MtrToPix(dPos);
//	dPos -= cPos;
//	float bitmapWidth(bitmap.getWidth());
//	float bitmapHeight(bitmap.getHeight());
//
//	if (													// If none of the below statements are true then the bitmap is within view of the camera.
//		(dPos.x > (camBotR.x - 1))		||	// Is the left side of the bitmap to the right of the camera?
//		((dPos.x + bitmapWidth - 1) < camTopL.x)	||	// Is the right side of the bitmap to the left side of the camera?
//		(dPos.y > (camBotR.y - 1))		||	// Is the top of the bitmap below the camera?
//		(dPos.y + bitmapHeight - 1) < camTopL.y)		// Is the bottom of the bitmap above the camera?
//		return false;
//
//	return true;
//}

void VE::DrawBitmap(const alx::Bitmap& bitmap, b2Vec2 dPos, b2Vec2 cPos, float angle, int flags)
{
	if (!bitmap)
		return;
	if (!VE::IsDrawable(bitmap, dPos, cPos))
		return;

	dPos = VE::MtrToPix(dPos);
	cPos = VE::MtrToPix(cPos);
	{
		b2Vec2 c;
		c.x = static_cast<float>(bitmap.getWidth()) / 2.0f;
		c.y = static_cast<float>(bitmap.getHeight()) / 2.0f;
		cPos = c - cPos;
	}

	dPos = VE::GameToScreenPosPix(VE::CLocator::GetRenderer()->GetCamera(), dPos);
	al_draw_rotated_bitmap(bitmap.get(), cPos.x, cPos.y, dPos.x, dPos.y, angle, flags);
}

void VE::DrawBitmap(const alx::Bitmap& bitmap, b2Vec2 dpos, int flags)
{
	if (!bitmap)
		return;
	if (!VE::IsDrawable(bitmap, dpos))// Is the position to be drawn at out of view of the camera?
		return;												// If so, don't waste CPU time drawing it.

	dpos = VE::MtrToPix(dpos);		// Meters->Pixels

	b2Vec2 drawPos(VE::GameToScreenPosPix(VE::CLocator::GetRenderer()->GetCamera(), dpos));
	al_draw_bitmap(bitmap.get(), drawPos.x, drawPos.y, flags);
}

bool VE::IsDrawable(const alx::Bitmap& bitmap, b2Vec2 dPos, b2Vec2 cPos)
{
		if (!bitmap)
			return false;

		VE::CCamera* cam = VE::CLocator::GetRenderer()->GetCamera();

		b2Vec2 camTopL = cam->TopLeftPosPix();
		// Copy the vector then add it's with and height to get its bottom right
		b2Vec2 camBotR(camTopL);
		camBotR.x += cam->Width();
		camBotR.y += cam->Height();

		// Convert to pix
		dPos = VE::MtrToPix(dPos);
		dPos -= cPos;
		float bitmapWidth(bitmap.getWidth());
		float bitmapHeight(bitmap.getHeight());

		if (													// If none of the below statements are true then the bitmap is within view of the camera.
			(dPos.x > (camBotR.x - 1))		||	// Is the left side of the bitmap to the right of the camera?
			((dPos.x + bitmapWidth - 1) < camTopL.x)	||	// Is the right side of the bitmap to the left side of the camera?
			(dPos.y > (camBotR.y - 1))		||	// Is the top of the bitmap below the camera?
			(dPos.y + bitmapHeight - 1) < camTopL.y)		// Is the bottom of the bitmap above the camera?
			return false;

		return true;
}