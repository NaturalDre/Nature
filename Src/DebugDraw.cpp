#include "DebugDraw.h"
#include <allegro5\allegro5.h>
#include <allegro5\allegro_primitives.h>
#include "Physics.h"

using namespace VE;

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	ALLEGRO_VERTEX* verts = new ALLEGRO_VERTEX[vertexCount];
	for (int i = 0; i < vertexCount; ++i)
	{
		b2Vec2 posPix = GameToScreenPosPix(m_cam, MtrToPix(vertices[i]));
		verts[i].x = posPix.x;
		verts[i].y = posPix.y;
		verts[i].z = 0;
		verts[i].color = al_map_rgb_f(color.r, color.g, color.b);
	}
	al_draw_prim(verts, NULL, NULL, 0, vertexCount, ALLEGRO_PRIM_LINE_LOOP);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	ALLEGRO_STATE state;

	al_store_state(&state, ALLEGRO_STATE_ALL);
	//al_set_blender(ALLEGRO_SRC_MINUS_DEST, ALLEGRO_ONE, ALLEGRO_ALPHA);
	//al_set_separate_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ONE, ALLEGRO_ZERO, ALLEGRO_DEST_MINUS_SRC, ALLEGRO_, ALLEGRO_ZERO);   d m s
	
	/* Settles */
	// al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ALPHA);
	// al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	// al_set_blender(ALLEGRO_ADD, ALLEGRO_INVERSE_ALPHA, ALLEGRO_INVERSE_ALPHA);


	// al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
	// al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ALPHA);
	// al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	// al_set_blender(ALLEGRO_ADD, ALLEGRO_INVERSE_ALPHA, ALLEGRO_ALPHA);

	//al_set_blender(ALLEGRO_SRC_MINUS_DEST, ALLEGRO_INVERSE_ALPHA, ALLEGRO_INVERSE_ALPHA);
	ALLEGRO_VERTEX* verts = new ALLEGRO_VERTEX[vertexCount];
	for (int i = 0; i < vertexCount; ++i)
	{
		b2Vec2 posPix = GameToScreenPosPix(m_cam, MtrToPix(vertices[i]));
		verts[i].x = posPix.x;
		verts[i].y = posPix.y;
		verts[i].z = 0;
		verts[i].color = al_map_rgba_f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		//verts[i].color = al_map_rgba_f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.8f);
	}
	al_draw_prim(verts, NULL, NULL, 0, vertexCount, ALLEGRO_PRIM_TRIANGLE_FAN);
	al_restore_state(&state);

	for (int i = 0; i < vertexCount; ++i)
	{
		b2Vec2 posPix = GameToScreenPosPix(m_cam, MtrToPix(vertices[i]));
		verts[i].x = posPix.x;
		verts[i].y = posPix.y;
		verts[i].z = 0;
		verts[i].color = al_map_rgba_f(color.r, color.g, color.b, 1.0f);
	}
	al_draw_prim(verts, NULL, NULL, 0, vertexCount, ALLEGRO_PRIM_LINE_LOOP);

	delete [] verts;
	verts = nullptr;

}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	//ALLEGRO_STATE state;

	//al_store_state(&state, ALLEGRO_STATE_BLENDER);
	//al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ALPHA);

	b2Vec2 centerPix = GameToScreenPosPix(m_cam, MtrToPix(center));
	ALLEGRO_COLOR col = al_map_rgba_f(color.r, color.g, color.b, 0.5f);

	al_draw_circle(centerPix.x, centerPix.y, MtrToPix(radius), col, 1);

	//al_restore_state(&state);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	//ALLEGRO_STATE state;

	//al_store_state(&state, ALLEGRO_STATE_BLENDER);
	//al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ALPHA);

	b2Vec2 centerPix = GameToScreenPosPix(m_cam, MtrToPix(center));
	ALLEGRO_COLOR col = al_map_rgba_f(color.r, color.g, color.b, 0.5f);

	al_draw_filled_circle(centerPix.x, centerPix.y, MtrToPix(radius), col);

	//al_restore_state(&state);

	al_draw_circle(centerPix.x, centerPix.y, MtrToPix(radius), col, 1);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	//b2Vec2 p1Pix = Utility::GameToScreenPosPix(Utility::mtrToPix(p1));
	//b2Vec2 p2Pix = Utility::GameToScreenPosPix(Utility::mtrToPix(p2));

	//ALLEGRO_COLOR col = al_map_rgb_f(color.r, color.g, color.b);
	//al_draw_line(p1.x, p1.y, p2.x, p2.y, col, 1);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1Pix;
	b2Vec2 p2Pix;

	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;

	p1Pix = GameToScreenPosPix(m_cam, MtrToPix(p1));

	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	p2Pix = GameToScreenPosPix(m_cam, MtrToPix(p2));

	al_draw_line(p1Pix.x, p1Pix.y, p2Pix.x, p2Pix.y, al_map_rgb_f(1.0f, 0.0f, 0.0f), 1);

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	p2Pix = GameToScreenPosPix(m_cam, MtrToPix(p2));

	al_draw_line(p1Pix.x, p1Pix.y, p2Pix.x, p2Pix.y, al_map_rgb_f(0.0f, 1.0f, 0.0f), 1);

}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	b2Vec2 point = GameToScreenPosPix(m_cam, MtrToPix(p));
	al_draw_rectangle(point.x, point.y, point.x, point.y, al_map_rgb_f(color.r, color.g, color.b), 4);
}

void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
	//char buffer[128];

	//va_list arg;
	//va_start(arg, string);
	//vsprintf_s(buffer, string, arg);
	//va_end(arg);

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//int w = 800;
	//int h = 600;
	//gluOrtho2D(0, w, h, 0);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();

	////glColor3f(0.9f, 0.6f, 0.6f);
	//glColor3f(0.9f, 0.6f, 0.6f);
	//glRasterPos2i(x, y); ////
	//int32 length = (int32)strlen(buffer);
	//for (int32 i = 0; i < length; ++i)
	//{
	//	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	//}

	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	b2Vec2 lB = GameToScreenPosPix(m_cam, MtrToPix(aabb->lowerBound));
	b2Vec2 uB = GameToScreenPosPix(m_cam, MtrToPix(aabb->upperBound));

	al_draw_rectangle(lB.x, lB.y, uB.x, uB.y, al_map_rgb_f(c.r, c.g, c.b), 1);
}