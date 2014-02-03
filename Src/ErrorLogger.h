#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H

#include <sstream>
#include <string>
#include <queue>

struct lua_State;

namespace VE
{
	class CEngine;
	class CErrorLogger
	{
	public:
		static void Export(lua_State* L);

	private:
		CErrorLogger(CEngine* engine = nullptr);
		~CErrorLogger(void);
	};
	void vShowMessage(const std::string& message, const std::string& file = "N/A", int line = 0);
}

#endif