#ifndef TILEDLUA_H
#define TILEDLUA_H

#include <string>
#include <map>
#include <lua.hpp>
#include <assert.h>
struct lua_State;
namespace Tiled
{
	/// \note The table MUST have a properties key). Table must be at top of stack before call.
	std::map<const std::string, const std::string> GetProperties(lua_State* L);
	/// \note The table is expected to be at the top of the stack.
	std::string GetTableValueStr(lua_State* L, const std::string& key);
	float GetTableValueF(lua_State* L, const std::string& key);
	int GetTableValueI(lua_State* L, const std::string& key);
	bool GetTableValueB(lua_State* L, const std::string& key);

	template <class T>
	T GetTableValueN(lua_State* L, const std::string& key)
	{
		//assert(L != nullptr);
		if (!L)
			return 0;
		// STK: table?
		if (!lua_istable(L, -1))
			return 0;
		// STK: table
		lua_pushstring(L, key.c_str());
		// STK: table pushedString(key)
		lua_gettable(L, -2);
		// STK: table receivedNumber(value)?
		T value = 0;
		if (lua_isnumber(L, -1))
		{
			// STK: table receivedNumber(value)
			value = static_cast<T>(lua_tonumber(L, -1));
		}
		lua_pop(L, 1);
		return value;
	}

}

#endif