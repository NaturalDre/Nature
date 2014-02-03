#include "Application.h"
#include "TetrisApp.h"
#include <iostream>
#include "tinyxml.h"
#include "tinystr.h"
#include <time.h>

using namespace std;
using namespace VE;

int main(int argc, const char* argv[])
{
	srand(time(static_cast<time_t>(0)));
	Tetris::CTetrisApp app;
	return app.Run();	
}