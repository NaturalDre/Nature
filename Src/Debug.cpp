#include "Debug.h"

#include <iostream>

CDebug::CDebug(void)
{

}

CDebug::~CDebug(void)
{

}

CDebug& CDebug::operator<<(const std::string& str)
{
	std::cerr << str;
	return *this;
}