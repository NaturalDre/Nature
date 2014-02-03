#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <assert.h>

#define VASSERT(exp, msg) assert(exp && msg); 
struct lua_State;

#define vMin(a,b) ((a < b) ? a : b )
#define vMax(a,b) ((a > b) ? a : b )
#define vContains(val, min, max) ((val >= min) && (val <= max))

void ShowError(const std::string&);
#define vError ShowError

std::vector<char> BufferFile(const std::string& filename);
void DoBuffer(lua_State* L, const char* buffer, size_t size);
void DoFile(lua_State* L, const std::string& filename);

struct ColRow 
{ 
	ColRow(size_t _col  = 0, size_t _row = 0): col(_col), row(_row) {} 
	size_t col, row; 
};
ColRow GetColRow(size_t cols, size_t id);
int GenerateEventID(char a, char b, char c, char d);
//inline size_t GenerateID(void) { static int count(0); return ++count; }

#endif