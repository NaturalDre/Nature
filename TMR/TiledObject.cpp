#include "TiledObject.h"
#include <lua.hpp>
#include "TiledLua.h"

Tiled::Object::Object(Object&& rhs)
	: m_name(rhs.m_name)
	, m_type(rhs.m_type)
	, m_x(rhs.m_x)
	, m_y(rhs.m_y)
	, m_width(rhs.m_width)
	, m_height(rhs.m_height)
	, m_visible(rhs.m_visible)
	, m_properties(std::move(rhs.m_properties))
	, m_isValid(rhs.IsValid()) { }

Tiled::Object::Object(lua_State* L)
	: m_x(0)
	, m_y(0)
	, m_width(0)
	, m_height(0)
	, m_visible(false)
	, m_isValid(false)
{
	m_name = GetTableValueStr(L, Tiled::Key::TiledObject::NAME);
	m_type = GetTableValueStr(L, Tiled::Key::TiledObject::TYPE);
	m_shape = GetTableValueStr(L, Tiled::Key::TiledObject::SHAPE);
	m_x = GetTableValueN<float>(L, Tiled::Key::TiledObject::X);
	m_y = GetTableValueN<float>(L, Tiled::Key::TiledObject::Y);
	m_width = GetTableValueN<float>(L, Tiled::Key::TiledObject::WIDTH);
	m_height = GetTableValueN<float>(L, Tiled::Key::TiledObject::HEIGHT);
	m_visible = GetTableValueB(L, Tiled::Key::TiledObject::VISIBLE);
	m_properties = std::move(GetProperties(L));
}
