#include "TiledLua.h"
#include <assert.h>

namespace Tiled
{

	std::map<const std::string, const std::string> GetProperties(lua_State*L)
	{
		// STK: -- table1
		lua_pushstring(L, "properties");
		// STK: -- table1 string
		lua_rawget(L, -2);
		// STK: -- table1 table2?
		if (!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			// STL: -- table1
			return std::map<const std::string, const std::string>();
		}
		// STK: -- table1 table2

		lua_insert(L, 1);
		// STK: table2 -- table1 
		lua_pushnil(L);
		// STK: table2 -- table1 keyNil
		std::map<const std::string, const std::string> props;
		while(lua_next(L, 1))
		{
			// STK: table2 -- table1 keyString? valueString?
			if (!lua_isstring(L, -2))
			{
				lua_pop(L, 2);
				// STK: table2 -- table1
				lua_remove(L, 1);
				// STK: -- table
				throw(std::exception("A key in properties is a non string value."));
			}
			else if(!lua_isstring(L, -1))
			{
				lua_pop(L, 2);
				// STK: table2 -- table1
				lua_remove(L, 1);
				// STK: -- table
				throw(std::exception("A value in properties contains a non string value."));
			}
			// STK: table2 -- table1 keyString valueString
			props.insert(std::pair<const std::string, const std::string>(lua_tostring(L, -2),lua_tostring(L, -1)));
			lua_pop(L, 1);
			// STK: table2 -- table1 keyString
		}
		// STK: table2 -- table1
		lua_remove(L, 1);
		// STK: -- table1
		return std::move(props);
	}

	std::string GetTableValueStr(lua_State* L, const std::string& key)
	{
		assert(L != nullptr);
		// STK: table?
		if (!lua_istable(L, -1))
			return "";
		// STK: table
		lua_pushstring(L, key.c_str());
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
		// STK: table
		return value;
	}

	float GetTableValueF(lua_State* L, const std::string& key)
	{
		assert(L != nullptr);
		// STK: table?
		if (!lua_istable(L, -1))
			return 0;
		// STK: table
		lua_pushstring(L, key.c_str());
		// STK: table pushedString(key)
		lua_gettable(L, -2);
		// STK: table receivedNumber(value)?
		float value;
		if (lua_isnumber(L, -1))
		{
			// STK: table receivedNumber(value)
			value = static_cast<float>(lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
		return value;
	}

	int GetTableValueI(lua_State* L, const std::string& key)
	{
		assert(L != nullptr);
		// STK: table?
		if (!lua_istable(L, -1))
			return 0;
		// STK: table
		lua_pushstring(L, key.c_str());
		// STK: table pushedString(key)
		lua_gettable(L, -2);
		// STK: table receivedNumber(value)?
		int value;
		if (lua_isnumber(L, -1))
		{
			// STK: table receivedNumber(value)
			value = lua_tointeger(L, -1);
		}
		lua_pop(L, 1);
		return value;
	}

	bool GetTableValueB(lua_State* L, const std::string& key)
	{
		assert(L != nullptr);
		// STK: table?
		if (!lua_istable(L, -1))
			return 0;
		// STK: table
		lua_pushstring(L, key.c_str());
		// STK: table pushedString(key)
		lua_gettable(L, -2);
		// STK: table receivedNumber(value)?
		bool value;
		if (lua_isboolean(L, -1))
		{
			// STK: table receivedNumber(value)
			value = lua_toboolean(L, -1);
		}
		lua_pop(L, 1);
		return value;
	}
}