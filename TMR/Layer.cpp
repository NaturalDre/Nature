#include "Layer.h"
#include <lua.hpp>
#include "TiledLua.h"

namespace Tiled
{
	void ILayer::ReadProperties(lua_State* L, ILayer* layer)
	{
		layer->m_properties = std::move(Tiled::GetProperties(L));
	}

	std::string ILayer::GetProperty(const std::string& prop)
	{
		auto iter = m_properties.find(prop);
		if (iter != m_properties.end())
			return iter->second;
		return "";
	}
}