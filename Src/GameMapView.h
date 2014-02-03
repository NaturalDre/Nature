#pragma once

#include "View.h"
#include "GameMap.h"

struct lua_State;
namespace VE
{
	class CGameMap;
	class CGameMapView: public IView
	{
	public:
		CGameMapView(CGameMap* map, size_t begin, size_t end);
		~CGameMapView(void);

		void SetBegin(size_t begin) { m_begin = begin; }
		void SetEnd(size_t end) { m_end = end; }

		static void Export(lua_State*);
	protected:
		void Draw(void);
		void RenderLayer(Tiled::CTileLayer* tilelayer);
		void RenderOrthogonal(Tiled::CTileLayer* tilelayer);
		void RenderIsometric(Tiled::CTileLayer* tilelayer);


		alx::Bitmap LoadTile(const size_t id);
		inline CGameMap* GetGameMap(void) const { return m_gameMap; }
	private:
		CGameMap* m_gameMap;
		/// The layer to start rendering at.
		size_t m_begin;
		/// The layer to end rendering at.
		size_t m_end;
	};
}