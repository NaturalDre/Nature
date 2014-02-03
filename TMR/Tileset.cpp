#include "Tileset.h"
#include "TiledLua.h"
#include <lauxlib.h>
#include <assert.h>
#include <allegro5\allegro5.h>
#include "Utility.h"
#include <lua.hpp>
using namespace Tiled;
using namespace luabind;


//static Tile::Properties ConvertTableToProperties(const luabind::object& table)
//{
//	Tile::Properties props;
//	for (luabind::iterator iter = luabind::iterator(table), end; iter != end; ++iter)
//	{
//		props.insert(std::pair<const std::string, const std::string>
//			(luabind::object_cast<const std::string>(iter.key())
//			, luabind::object_cast<std::string>(*iter)));
//	}
//	return std::move(props);
//}

//CTileset::CTileset(const luabind::adl::object& tileset)
//	: m_isValid(false)
//{
//	LoadDefaults();
//	if(!tileset.is_valid() || type(tileset) != LUA_TTABLE)
//		return;
//
//	try
//	{
//		LoadTilesetProperties(*this, tileset);
//		m_tiles = LoadTilesetTiles(*this, tileset["tiles"]);
//		m_isValid = true;
//	}
//	catch(const luabind::error& e)
//	{
//		lua_pop(e.state(), 1);
//	}
//	catch(...) { }
//}
CTileset::CTileset(lua_State* L)
{
	LoadDefaults();
	if (!lua_istable(L, -1))
		return;
	try
	{
		LoadTilesetProperties(*this, L);
	}
	catch(...)
	{

	}
}

CTileset::~CTileset(void)
{

}

//void CTileset::LoadTilesetProperties(CTileset& ts, const luabind::adl::object& tileset)
//{
//	ts.m_name = object_cast<std::string>(tileset["name"]);
//	ts.m_firstGid = object_cast<size_t>(tileset["firstgid"]);
//	ts.m_tileWidth = object_cast<size_t>(tileset["tilewidth"]);
//	ts.m_tileHeight = object_cast<size_t>(tileset["tileheight"]);
//	ts.m_spacing = object_cast<size_t>(tileset["spacing"]);
//
//	ts.m_source = object_cast<std::string>(tileset["image"]);
//	ts.m_imageWidth = object_cast<size_t>(tileset["imagewidth"]);
//	ts.m_imageHeight = object_cast<size_t>(tileset["imageheight"]);
//	//ts.m_trans = object_cast<std::string>(tileset["transparentcolor"]);
//	ts.m_trans = "";
//
//	ts.m_tilesAcross = ts.m_imageWidth / (ts.m_tileWidth + ts.m_spacing);
//	assert((ts.m_tileHeight + ts.m_spacing));
//	ts.m_tilesDown = ts.m_imageHeight / (ts.m_tileHeight + ts.m_spacing);
//	ts.m_lastGid = ts.m_firstGid + (((ts.m_imageWidth / (ts.m_tileWidth + ts.m_spacing)) * (ts.m_imageHeight / (ts.m_tileHeight + ts.m_spacing)))-1);
//}

void CTileset::LoadTilesetProperties(CTileset& ts, lua_State* L)
{
	if (!L || !lua_istable(L, -1))
		return;

	ts.m_name = GetTableValueStr(L, Tiled::Key::TileSet::NAME);
	ts.m_firstGid = GetTableValueN<size_t>(L, Tiled::Key::TileSet::FIRSTGID);
	ts.m_tileWidth = GetTableValueN<size_t>(L, Tiled::Key::TileSet::TILEWIDTH);
	ts.m_tileHeight = GetTableValueN<size_t>(L, Tiled::Key::TileSet::TILEHEIGHT);
	ts.m_spacing = GetTableValueN<size_t>(L, Tiled::Key::TileSet::SPACING);
	
	ts.m_source = GetTableValueStr(L, Tiled::Key::TileSet::IMAGE);
	ts.m_imageWidth = GetTableValueN<size_t>(L, Tiled::Key::TileSet::IMAGEWIDTH);
	ts.m_imageHeight = GetTableValueN<size_t>(L, Tiled::Key::TileSet::IMAGEHEIGHT);
	ts.m_trans = GetTableValueStr(L, Tiled::Key::TileSet::TRANSPARENTCOLOR);

	ts.m_tilesAcross = ts.m_imageWidth / (ts.m_tileWidth + ts.m_spacing);
	// I can not remember why I put this assert here...
	assert((ts.m_tileHeight + ts.m_spacing));
	ts.m_tilesDown = ts.m_imageHeight / (ts.m_tileHeight + ts.m_spacing);
	ts.m_lastGid = ts.m_firstGid + (((ts.m_imageWidth / (ts.m_tileWidth + ts.m_spacing)) * (ts.m_imageHeight / (ts.m_tileHeight + ts.m_spacing)))-1);
}

void CTileset::LoadDefaults()
{
	m_name = "";
	m_firstGid = 0;
	m_tileWidth = 0;
	m_tileHeight = 0;
	m_spacing = 0;
	m_source = "";
	m_imageWidth = 0;
	m_imageHeight = 0;
	m_trans = "";
	m_tileHeight = 0;
	m_tilesDown = 0;
	m_lastGid = 0;
}

//std::list<Tile> CTileset::LoadTilesetTiles(CTileset& ts, const luabind::adl::object& data)
//{
//	std::list<Tile> tiles;
//
//	if(!data.is_valid() || type(data) != LUA_TTABLE)
//		return tiles;
//
//
//
//	for (luabind::iterator iter = luabind::iterator(data), end; iter != end; ++iter)
//	{
//		luabind::object tile = *iter;
//
//		tiles.push_back(Tile(luabind::object_cast<int>(tile["id"]), ConvertTableToProperties(tile["properties"])));	
//	}
//	return std::move(tiles);
//}

CTileset* CTileset::ContainsGid(const std::vector<CTileset*>& tilesets, size_t id)
{
	for (auto iter = tilesets.begin(); iter != tilesets.end(); ++iter)
	{
		if ((*iter)->ContainsGid(id))
			return *iter;
	}
	return nullptr;
}

namespace Tiled
{
	size_t DataValue(lua_State* L, size_t layer, size_t index)
	{
		assert(L != nullptr);

		lua_getglobal(L, "GetLayerDataVal");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			throw(std::exception("Could not get function GetLayerDataVal"));
		}
		// STK: func
		lua_pushinteger(L, layer);
		lua_pushinteger(L, index);
		// STK: func int int
		if (lua_pcall(L, 2, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if(!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t GetNumOfTilesets(lua_State* L)
	{
		assert(L != nullptr);

		lua_getglobal(L, "GetNumberOfTilesets");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			throw(std::exception("GetNumberOfTilesets could not be found."));
		}
		// STK: func
		if (lua_pcall(L, 0, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		return val;
	}

	std::string Version(lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetMapVersion");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
			//throw(std::exception("GetMapWidth is not a function."));
		}
		// STK: func
		if (lua_pcall(L, 0, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isstring(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		std::string val = lua_tostring(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	std::string Orientation(lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetMapOrientation");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
			//throw(std::exception("GetMapWidth is not a function."));
		}
		// STK: func
		if (lua_pcall(L, 0, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isstring(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		std::string val = lua_tostring(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t MapWidth(lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetMapWidth");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
			//throw(std::exception("GetMapWidth is not a function."));
		}
		// STK: func
		if (lua_pcall(L, 0, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t MapHeight(lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetMapHeight");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		if (lua_pcall(L, 0, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t TileWidth(lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetMapTileWidth");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		if (lua_pcall(L, 0, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t TileHeight(lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetMapTileHeight");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		if (lua_pcall(L, 0, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t LayerCount(lua_State* L)
	{
		// STK: 
		lua_getglobal(L, "GetNumberOfLayers");
		// STK: func?
		if(!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
			//throw(std::exception(("GetNumberOfLayers" " was not found or is not a function").c_str()));
		}
		// STK: func
		if(lua_pcall(L, 0, 1, 0) != 0)
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			return 0;
			//throw(std::exception(("GetNumberOfLayers" + "\n" + error).c_str()));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int amount = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return amount;
	}

	std::string LayerType(lua_State* L, size_t element)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetLayerType");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return "";
			//throw(std::exception((Var::L_ISTILELAYER + " was not found or is not a function.").c_str()));
		}
		// STK: func
		lua_pushinteger(L, element);
		// STK: func int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			return "";
			//throw(std::exception((Var::L_ISTILELAYER + "\n" + error).c_str()));
		}
		// STK: string?
		if (!lua_isstring(L, -1))
		{
			lua_pop(L, 1);
			return false;
		}
		// STK: bool
		std::string val = lua_tostring(L, -1);
		lua_pop(L, 1);
		// STK: 
		return val;
	}

	bool PushTiledObject(lua_State* L, size_t layer, size_t index)
	{
		assert(L != nullptr);

		lua_getglobal(L, "GetObjLayerObject");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			throw(std::exception("Could not get function GetLayerDataVal"));
		}
		// STK: func
		lua_pushinteger(L, layer);
		lua_pushinteger(L, index);
		// STK: func int int
		if (lua_pcall(L, 2, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: table?
		if(!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			return false;
		}

		return true;
	};

	bool PushLayer(lua_State* L, size_t layer)
	{
		assert(L != nullptr);

		lua_getglobal(L, "GetLayerObject");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			throw(std::exception("Could not get function GetLayerObject"));
		}
		// STK: func
		lua_pushinteger(L, layer);
		//lua_pushinteger(L, index);
		// STK: func int int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: table?
		if(!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			return false;
		}
		// STK:
		return true;
	}

	size_t NumberOfObjects(lua_State* L, size_t layer)
	{
		assert(L != nullptr);

		lua_getglobal(L, "GetNumOfObjLayerObjects");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		lua_pushinteger(L, layer);
		// STK: func int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		return val;
	}

	std::string GetTilesetImage(size_t index, lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetTilesetImage");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		lua_pushinteger(L, index);
		// STK: func int
		if (lua_pcall(L, 1, 1,0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: string?
		if (!lua_isstring(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: string
		std::string val = lua_tostring(L, -1);
		lua_pop(L, 1);
		// STK:
		return std::move(val);	
	}

	size_t GetTilesetImgW(const size_t index, lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetTilesetImageWidth");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		lua_pushinteger(L, index);
		// STK: func int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t GetTilesetImgH(const size_t index, lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetTilesetImageHeight");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		lua_pushinteger(L, index);
		// STK: func int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	std::string GetTilesetTrans(const size_t index, lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetTilesetTrans");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		lua_pushinteger(L, index);
		// STK: func int
		if (lua_pcall(L, 1, 1,0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: string?
		if (!lua_isstring(L, -1))
		{
			lua_pop(L, 1);
			return "";
		}
		// STK: string
		std::string val = lua_tostring(L, -1);
		lua_pop(L, 1);
		return std::move(val);	
	}

	size_t GetTilesetSpacing(const size_t index, lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetTilesetSpacing");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		lua_pushinteger(L, index);
		// STK: func int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	std::string GetTilesetName(lua_State* L)
	{
		assert(L != nullptr);
		// STK: table?
		if (!lua_istable(L, -1))
			return "";
		// STK: table
		lua_pushstring(L, "name");
		// STK: table stringPushed(key)
		lua_gettable(L, -2);
		// STK: table stringReceived(value)?
		std::string value;
		if (lua_isstring(L, -1))
		{
			// STK: table stringReceived(value)
			value = lua_tostring(L, -1);
		}
		// STK: table stringReceived?
		lua_pop(L, 1);
		// STK: table
		return value;
	}

	size_t GetTilesetFirstGid(size_t index, lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetTilesetFirstGid");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		lua_pushinteger(L, index);
		// STK: func int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t GetTilesetTileWidth(size_t index, lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetTilesetTileWidth");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			throw(std::exception("Could not get function GetTilesetTileWidth"));
		}
		// STK: func
		lua_pushinteger(L, index);
		// STK: func int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}

	size_t GetTilesetTileHeight(size_t index, lua_State* L)
	{
		assert(L != nullptr);
		// STK:
		lua_getglobal(L, "GetTilesetTileHeight");
		// STK: func?
		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: func
		lua_pushinteger(L, index);
		// STK: func int
		if (lua_pcall(L, 1, 1, 0))
		{
			const char* error = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw(std::exception(error));
		}
		// STK: int?
		if (!lua_isnumber(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		// STK: int
		int val = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK:
		return val;
	}
}