#include "Tilelayer.h"
#include "MapFile.h"
#include <lua.hpp>
#include <assert.h>
#include "TiledLua.h"

using namespace Tiled;

CTileLayer::CTileLayer(lua_State* L, const CMapFile& mapFile)
{
	// STK: -- map.layers[i] // i = any positive integer.
	if (!L || !lua_istable(L, -1))
		return;

	SetWidth(mapFile.GetWidth());
	SetHeight(mapFile.GetHeight());

	SetName(GetTableValueStr(L, "name"));
	ReadProperties(L, this);

	// STK: -- map.layers[]
	lua_pushstring(L, "data");
	// STK: -- map.layers[] pushedString
	lua_gettable(L, -2);
	// STK: -- map.layers[] map.layers[].data
	lua_insert(L, 1);
	// STK: map.layers[].data -- map.layers[]
	m_data.resize(GetHeight(), std::vector<size_t>(GetWidth(), 0));
	lua_pushnil(L);
	// STK: map.layers[].data -- map.layers[] nil(key)
	for (int index = 0, row = 0, col = 0; lua_next(L, 1) != 0; ++index)
	{
		// STK: map.layers[].data -- map.layers[] key value
		if (col >= static_cast<int>(GetWidth()))
		{
			++row;
			col = 0;
		}
		m_data[row][col++] = lua_tointeger(L, -1);
		lua_pop(L, 1);
		// STK: map.layers[].data -- map.layers[] key
	}
	// STK: map.layers[].data -- map.layers[]
	lua_remove(L, 1);
	// STK: -- map.layers[]
}

void CTileLayer::SetDataVal(size_t row, size_t col, size_t value)
{
	if (!m_data.empty() && row < m_data.size() && col < m_data.front().size())
		m_data[row][col] = value;
}

size_t CTileLayer::GetDataVal(size_t row, size_t col)
{
	if (!m_data.empty() && row < m_data.size() && col < m_data.front().size())
		return m_data[row][col];
	return 0;
}

void CTileLayer::ReplaceAll(size_t oldGid, size_t newGid)
{
	for(size_t row = 0; row < m_data.size(); ++row)
	{
		for(size_t col = 0; col < m_data.front().size(); ++col)
		{
			if (GetDataVal(row, col) == oldGid)
				SetDataVal(row, col, newGid);
		}
	}
}