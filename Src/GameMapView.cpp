#include "GameMapView.h"
#include "Locator.h"
#include "Render.h"
#include "Common.h"
#include "Draw.h"
namespace VE
{
	CGameMapView::CGameMapView(CGameMap* map, size_t begin, size_t end)
		: IView(-10)
		, m_gameMap(map)
		, m_begin(begin)
		, m_end(end)
	{

	}

	CGameMapView::~CGameMapView(void)
	{

	}

	void CGameMapView::Draw(void)
	{
		if (!GetGameMap())
			return;
		for (size_t i = m_begin; (i < GetGameMap()->GetTileLayers().size()) && (i <= m_end); ++i)
			RenderLayer(GetGameMap()->GetTileLayers().at(i));
	}
	void CGameMapView::RenderLayer(Tiled::CTileLayer* tilelayer)
	{
		// Top left x and y position of the camera
		const float tlx = CLocator::GetRenderer()->GetCamera()->TopLeftPosPix().x;
		const float tly = CLocator::GetRenderer()->GetCamera()->TopLeftPosPix().y;	

		if (GetGameMap()->GetOrientation() == "orthogonal")
			RenderOrthogonal(tilelayer);
		else if (GetGameMap()->GetOrientation() == "staggered")
			RenderIsometric(tilelayer);
		else
			return;
	}

	void CGameMapView::RenderOrthogonal(Tiled::CTileLayer* tilelayer)
	{
		const float tlx = CLocator::GetRenderer()->GetCamera()->TopLeftPosPix().x;
		const float tly = CLocator::GetRenderer()->GetCamera()->TopLeftPosPix().y;	

		alx::Bitmap tile;
		size_t prevID(0);

		const int startCol(static_cast<int>(tlx / GetGameMap()->GetTileWidth()));
		const int startRow(static_cast<int>(tly / GetGameMap()->GetTileHeight()));

		const int endCol = startCol + (VE::GetDisplayWidth() / GetGameMap()->GetTileWidth()) + 2; // +2 is buffer otherwise last col won't draw
		const int endRow = startRow + (VE::GetDisplayHeight() / GetGameMap()->GetTileHeight()) + 2; // +2 is buffer otherwise last row won't draw

		for (int row = startRow; row < endRow; ++row)
		{
			for (int col = startCol; col < endCol; ++col)
			{
				int id = tilelayer->GetDataVal(row, col);
				if (id == 0)
					continue;
				if (id != prevID)
				{
					prevID = id;
					tile = LoadTile(id);
				}

				if (prevID != id)
					prevID = id;

				int dx = col * GetGameMap()->GetTileWidth();
				int dy = row * GetGameMap()->GetTileHeight();

				if (tile)
					DrawBitmap(tile, VE::PixToMtr(b2Vec2(static_cast<float>(dx), static_cast<float>(dy))));
			}
		}
	}

	void CGameMapView::RenderIsometric(Tiled::CTileLayer* tilelayer)
	{
		const float tlx = CLocator::GetRenderer()->GetCamera()->TopLeftPosPix().x;
		const float tly = CLocator::GetRenderer()->GetCamera()->TopLeftPosPix().y;

		const int startCol(static_cast<int>(tlx / GetGameMap()->GetTileWidth()));
		const int startRow(static_cast<int>(tly / GetGameMap()->GetTileHeight()));

		alx::Bitmap tile;
		size_t prevID(0);

		int x = 0, y = 0;
		int offsetX = VE::GetDisplayWidth() - ( GetGameMap()->GetTileWidth() * GetGameMap()->GetWidth()) / 2.0f;
		int offsetY = VE::GetDisplayHeight() / 2.0f;

		//for (int i = 0; i < GetGameMap()->GetHeight(); ++i)
		//{
		//	if (i % 2 != 0)
		//		offsetX = GetGameMap()->GetTileWidth() / 2;
		//	else
		//		offsetX = 0;

		//	for (int j = 0; j < GetGameMap()->GetWidth(); ++j)
		//	{
		//		int id = tilelayer->GetDataVal(i, j);
		//		tile = LoadTile(id);
		//		if (tile)
		//		{
		//			x = (j * GetGameMap()->GetTileWidth()) + offsetX;
		//			y = (i * GetGameMap()->GetTileHeight()) / 2;
		//			DrawBitmap(tile, VE::PixToMtr(b2Vec2(static_cast<float>(x), static_cast<float>(y))));
		//		}
		//	}
		//}



		for (int i = 0; i < static_cast<int>(GetGameMap()->GetHeight()); ++i) {
			for(int j = GetGameMap()->GetWidth(); j >= 0; --j) {
				//x = offsetX + ((j * GetGameMap()->GetTileWidth() / 2.0f) + (i * GetGameMap()->GetTileWidth() / 2.0f));
				//y = offsetY + ((i * GetGameMap()->GetTileHeight() / 2.0f) - (j * GetGameMap()->GetTileHeight() / 2.0f));

				x = (j * GetGameMap()->GetTileWidth() / 2) + (i * GetGameMap()->GetTileWidth() / 2);
				y = (i * GetGameMap()->GetTileHeight() / 2) - (j * GetGameMap()->GetTileHeight() / 2);
				
				int id = tilelayer->GetDataVal(i, j);
				tile = LoadTile(id);
				if (tile)
				{
					DrawBitmap(tile, VE::PixToMtr(b2Vec2(static_cast<float>(x), static_cast<float>(y))));
				}
				//_tiles[_map[i * _mapHeight + j]]->draw(x, y, 0, _zoom, _zoom);
			}
		}
	}

	alx::Bitmap CGameMapView::LoadTile(const size_t id)
	{
		Tiled::CTileset* ts = Tiled::CTileset::ContainsGid(GetGameMap()->GetTilesets(), id);
		if (!ts)
			return alx::Bitmap();

		// I wish i could make image a const, but alx::Bitmap's constructor requires
		// a non-const. Maybe I'll fix that.
		alx::Bitmap image = GetGameMap()->GetTilesetImage(ts->Name());

		if (!image)
			return alx::Bitmap();

		const ColRow  cr = GetColRow(ts->TilesAcross(), (id - ts->FirstGid()) + 1);

		alx::Bitmap bitmap = alx::Bitmap(image, 
			(cr.col - 1) * (ts->ImageWidth() + ts->Spacing()),
			(cr.row - 1) * (ts->ImageHeight() + ts->Spacing()),
			ts->TileWidth(),
			ts->TileHeight());

		return std::move(bitmap);
	}

	//void CGameMapView::Export(lua_State* L)
	//{
	//	using namespace luabind;
	//	module(L)
	//		[
	//			class_<CGameMapView>("CGameMapView")
	//			.def(constructor<CGameMap*, size_t, size_t>())
	//		];
	//}
}