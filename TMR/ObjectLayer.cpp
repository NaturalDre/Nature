#include "ObjectLayer.h"
#include "TiledLua.h"
#include <lua.hpp>
#include <assert.h>
#include <algorithm>

using namespace Tiled;

CObjectLayer::CObjectLayer(lua_State* L)
{
	// STK: -- map.layers[i]
	if (!L || !lua_istable(L, -1))
		return;
	SetName(Tiled::GetTableValueStr(L, "name"));
	ReadProperties(L, this);

	lua_pushstring(L, "objects");
	// STK: -- map.layers[i] pushedString
	lua_gettable(L, -2);
	// STK: -- map.layers[i] map.layers[i].objects
	lua_insert(L, 1);
	// STK: map.layers[i].objects -- map.layers[i]
	lua_pushnil(L);
	// STK: map.layers[i].objects -- map.layers[i] nil
	for (size_t i = 0; lua_next(L, 1); ++i)
	{
	// STK: map.layers[i].objects -- map.layers[i] key val
		m_objects.push_back(Tiled::Object(L));
		lua_pop(L, 1);
	// STK: map.layers[i].objects -- map.layers[i] key
	}
	// STK: map.layers[i].objects -- map.layers[i]
	lua_remove(L, 1);
	// STK: -- map.layers[i]
}

const Object* CObjectLayer::FindObject(const std::string& name) const
{
	for(auto iter = m_objects.begin(); iter != m_objects.end(); ++iter)
	{
		if (iter->Name() == name)
			return &(*iter);
	}
	return nullptr;
}

std::list<const Object*> CObjectLayer::FindObjects(const std::string& name) const
{
	std::list<const Object*> objects;
	for (auto iter = m_objects.begin(); iter != m_objects.end(); ++iter)
	{
		if (iter->Name() == name)
			objects.push_back(&(*iter));
	}
	return std::move(objects);
}