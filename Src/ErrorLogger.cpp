#include "ErrorLogger.h"
#include "Engine.h"
#include <boost\lexical_cast.hpp>
#include <iostream>
#include <allegro5\allegro.h>
#define vTOSTRING boost::lexical_cast<std::string>

namespace VE
{
	void vShowMessage(const std::string& message, const std::string& file, int line)
	{
		std::cout << std::endl << "[" << al_current_time() << "]\t" <<
		 " MSG: " << message << " FILE: " << file << " LINE: " << line << std::endl;
	}
}