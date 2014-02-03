#include "GameMap.h"
#include "Render.h"
#include "Utility.h"
#include <lua.hpp>
#include "Camera.h"
#include <allegro5\allegro5.h>
#include "GameMapView.h"

namespace VE
{
	CGameMap::CGameMap(IGameManager* gameManager)
		: m_gameManager(gameManager)
		, m_playerLayer(0)
		, m_playerSpawn(0,0)
		, m_lowerView(nullptr)
		, m_upperView(nullptr)

	{
		m_parser = CMapParser(gameManager, this);
	}

	CGameMap::ConstTileLayerIter CGameMap::FindTileLayer(const std::string& name)
	{
		for( ConstTileLayerIter iter = GetTileLayers().begin(); iter != GetTileLayers().end(); ++iter)
		{
			if ((*iter)->GetName() == name)
				return iter;
		}
		return GetTileLayers().end();
	}

	int CGameMap::FindTileLayerIndex(const std::string& name)
	{
		auto layerIter = FindTileLayer(name);
		size_t index(0);
		for (auto iter = GetTileLayers().begin(); iter != GetTileLayers().end(); ++iter, ++index)
		{
			if (iter == layerIter)
				return index;
		}
		return -1;
	}

	bool CGameMap::Read(const std::string& filename)
	{
		if (!Tiled::CMapFile::Read(filename))
			return false;
		
		// For now I'm just having lowerView render every layer until I actual design
		// a map that makes use of upperView.
		if (m_lowerView)
		{
			delete m_lowerView;
			m_lowerView = nullptr;
		}
		m_lowerView = new CGameMapView(this, 0, GetTileLayers().size());

		std::for_each(GetTilesets().begin(), GetTilesets().end(), [&](Tiled::CTileset* tileset)
		{
			if (!tileset->Name().empty() && !tileset->Source().empty())
				m_tilesetImages[tileset->Name()] = std::move(alx::Bitmap(("Images/Tilesets/" + tileset->Source()).c_str()));
		});

		m_parser.Parse();
		return true;
	}

	const alx::Bitmap CGameMap::GetTilesetImage(const std::string& name) const
	{
		auto iter = m_tilesetImages.find(name);
		if (iter != m_tilesetImages.end())
			return iter->second;
		return alx::Bitmap();
	}

	//void CGameMap::Export(lua_State* L)
	//{
	//	using namespace luabind;
	//	module(L)
	//		[
	//			class_<CGameMap, bases<Tiled::CMapFile>>("CGameMap")
	//			.def(constructor<IGameManager*>())
	//			.def("Read", &CGameMap::Read)
	//		];
	//}
}