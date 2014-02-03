#pragma once

#include <array>

//static std::array<std::array<int, 2>, 3> TETRIS_L;

const std::array<const std::array<int, 4>,4> TETRIS_L1 = 
{ 
	{ 
		{ {0,0,1} }, 
		{ {1,1,1} },
		{ {0,0,0} }
	} 
};

const std::array<const std::array<int, 4>,4> TETRIS_L2 = 
{ 
	{ 
		{ {0,1,0} }, 
		{ {0,1,0} },
		{ {0,1,1} }
	} 
};

const std::array<const std::array<int, 4>,4> TETRIS_L3 = 
{ 
	{ 
		{ {0,0,0} }, 
		{ {1,1,1} },
		{ {1,0,0} }
	} 
};

const std::array<const std::array<int, 4>,4> TETRIS_L4 = 
{ 
	{ 
		{ {1,1,0} }, 
		{ {0,1,0} },
		{ {0,1,0} }
	} 
};

const std::array<std::array<const std::array<int, 4>,4>, 4> TETRIS_L = { TETRIS_L1, TETRIS_L2, TETRIS_L3, TETRIS_L4 };