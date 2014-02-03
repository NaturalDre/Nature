#include "MapFile.h"
#include <assert.h>
#include <lua.hpp>
#include "Tilelayer.h"
#include "ObjectLayer.h"
#include "TiledLua.h"
#include "Utility.h"
#include <Bitmap.hpp>
#include "Render.h"
#include <boost\foreach.hpp>
#include "lualib.h"
#include <algorithm>

using namespace Tiled;

namespace Tiled
{
	std::string GetVersion(lua_State* L);
}

bool LoadTiledLib(lua_State* L)
{
	try { DoFile(L, "Scripts/map/TiledLib.lua"); return true; }
	catch (...) { return false; }
}

CMapFile::CMapFile(void)
{
	m_width = 0;
	m_height = 0;
	m_tileHeight = 0;
	m_tileWidth = 0;
	m_valid = false;
}

CMapFile::~CMapFile(void)
{
	Reset();
}
void CMapFile::LoadMapData(CMapFile& map, lua_State* L)
{
	if (!L || !lua_istable(L, -1))
		return;
	map.m_version = Tiled::GetTableValueStr(L, "version");
	map.m_orientation = Tiled::GetTableValueStr(L, "orientation");
	map.m_width = Tiled::GetTableValueF(L, "width");
	map.m_height = Tiled::GetTableValueF(L, "height");
	map.m_tileWidth = Tiled::GetTableValueF(L, "tilewidth");
	map.m_tileHeight = Tiled::GetTableValueF(L, "tileheight");
	map.m_properties = Tiled::GetProperties(L);
}

void CMapFile::LoadLayers(CMapFile& map, lua_State* L)
{
	// STK: table(map)?(Root of the map table.)
	if (!lua_istable(L, -1))
		return;
	// STK: table(map)
	{
		lua_pushstring(L, "tilesets");
		// STK: table(map) pushedString(key)
		lua_gettable(L, -2);
		//STK: table(map) receivedTable?(key)
		if (!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			// STK: table(map)
			return;
		}
		// STK: table(map) table(map.tilesets)
		for (size_t i = 1; i <= luaL_getn(L, -1); ++i)
		{
			lua_pushinteger(L, i);
			// STK: table(map) table(map.tilesets) int(key)
			lua_gettable(L, -2);
			// STK: table(map) table(map.tilesets) table(map.tilesets[i])?
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 2);
				// STK: table(map)
				return;
			}
			// STK: table(map) table(map.tilesets) table(map.tilesets[i])
			map.m_tilesets.push_back(new CTileset(L));
			lua_pop(L, 1);
			// STK: table(map) table(map.tilesets)
		}
		// STK: table(map) table(map.tilesets)
		lua_pop(L, 1);
		// STK: table(map)
	}
	// STK: table(map)
	lua_pushstring(L, "layers");
	// STK: table(map) pushedString(key)
	lua_gettable(L, -2);
	// STK: table(map) table(map.layers)
	for (size_t i = 1, max = luaL_getn(L, -1); i <= max; ++i)
	{
		lua_pushinteger(L, i);
		// STK: table(map) table(map.layers) int(key)
		lua_gettable(L, -2);
		// STK: table(map) table(map.layers) table(map.layers[i])?
		if (!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			// STK: table(map) table(map.layers)
			continue;
		}
		// STK: table(map) table(map.layers) table(map.layers[i])
		lua_pushstring(L, "type");
		// STK: table(map) table(map.layers) table(map.layers[i]) pushedString
		lua_gettable(L, -2);
		// STK: table(map) table(map.layers) table(map.layers[i]) receivedString?
		if (!lua_isstring(L, -1))
		{
			lua_pop(L, 2);
		// STK: table(map) table(map.layers)
			continue;
		}
		// STK: table(map) table(map.layers) table(map.layers[i]) receivedString
		const std::string type = lua_tostring(L, -1);
		lua_pop(L, 1);
		// STK: table(map) table(map.layers) table(map.layers[i])
		if (type == "tilelayer")
			map.m_tileLayers.push_back(new CTileLayer(L, map));
		else if (type == "objectgroup")
			map.m_objectLayers.push_back(new CObjectLayer(L));
		lua_pop(L, 1);
		// STK: table(map) table(map.layers)
	}
	lua_pop(L, 1);
	// STK: table(map)
}

bool CMapFile::Read(const std::string& mapFile)
{
	Reset();
	// Open an lua state
	lua_State* L = lua_open();
	if (!L)
		return false;

	luaL_openlibs(L);


	// Tiled exports map files beginning with "return {" for whatever reason, so
	// I need to correct that here so each map file does not need to be edited by 
	// hand to be read.
	{
		std::vector<char> bufferedFile = BufferFile(mapFile);
		if (!bufferedFile.empty())
		{
			bufferedFile.erase(bufferedFile.begin(), bufferedFile.begin() + 6);
			std::string map = "Map =";
			bufferedFile.insert(bufferedFile.begin(), map.begin(), map.end());

			try { DoBuffer(L, bufferedFile.data(), bufferedFile.size()); }
			catch (const std::exception& e)
			{
				// TO DO: Add error handling code.
			}
		}
	}

	// At this point a global named 'Map' should exist. It is the exported Tiled Lua map file.
	lua_getglobal(L, "Map");
	// STK: table?
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		//STK:
		return false;
	}
	LoadMapData(*this, L);
	LoadLayers(*this, L);
	lua_close(L);
	return m_valid = true;
}

const std::string CMapFile::GetProperty(const std::string& key)
{
	auto iter = m_properties.find(key);
	if (iter != m_properties.end())
		return iter->second;
	return "";
}

CTileset* CMapFile::GetTileset(const std::string& name) const
{
	for(auto iter = m_tilesets.begin(); iter != m_tilesets.end(); ++iter)
	{
		if ((*iter)->Name() == name)
			return *iter;
	}
	return nullptr;
}

const Object* CMapFile::FindObject(const std::string& name) const
{
	for (auto iter = m_objectLayers.begin(); iter != m_objectLayers.end(); ++iter)
	{
		const Object* obj = (*iter)->FindObject(name);
		if (obj)
			return obj;
	}
	return nullptr;
}

void CMapFile::Reset(void)
{
	m_version.clear();
	m_orientation.clear();
	m_properties.clear();
	m_width = 0;
	m_height = 0;
	m_tileWidth = 0;
	m_tileHeight = 0;

	BOOST_FOREACH(CTileLayer* tilelayer, m_tileLayers)
		delete tilelayer;
	m_tileLayers.clear();

	BOOST_FOREACH(CObjectLayer* objectlayer, m_objectLayers)
		delete objectlayer;
	m_objectLayers.clear();

	BOOST_FOREACH(CTileset* tileset, m_tilesets)
		delete tileset;
	m_tilesets.clear();

	m_valid = false;
}

namespace Tiled
{
	std::string GetVersion(lua_State* L)
	{
		assert(L != nullptr);
		// STK: table?
		if (!lua_istable(L, -1))
			return "";
		// STK: table
		lua_pushstring(L, "version");
		// STK: table pushedString(key)
		lua_gettable(L, -2);
		// STK: table receivedString(value)?
		std::string value;
		if (lua_isstring(L, -1))
		{
			// STK: table stringReceived(value)
			value = lua_tostring(L, -1);
		}
		lua_pop(L, 1);
		return value;
	}
}