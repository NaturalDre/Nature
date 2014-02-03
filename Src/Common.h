#ifndef COMMON_H
#define COMMON_H

#include "Utility.h"

namespace VE
{
	enum DIRECTION
	{
		  UP = 1
		, DOWN
		, LEFT
		, RIGHT
	};

	typedef unsigned short ushort;

	const short CATEGORY_PLAYER				= 0x0001;
	const short CATEGORY_ENEMY				= 0x0002;
	const short CATEGORY_SCENERY			= 0x0004;
	const short CATEGORY_PLAYERPROJECTILE	= 0x0008;
	const short CATEGORY_ENEMYPROJECTILE	= 0x0010;

	const short MASK_PLAYER				= CATEGORY_ENEMY | CATEGORY_SCENERY;
	const short MASK_ENEMY				= CATEGORY_PLAYER | CATEGORY_SCENERY;
	const short MASK_SCENERY			= -1; // Everything
	const short MASK_PLAYERPROJECTILE	= CATEGORY_ENEMY | CATEGORY_SCENERY | CATEGORY_ENEMYPROJECTILE;
	const short MASK_ENEMYPROJECTILE	= CATEGORY_PLAYER | CATEGORY_SCENERY | CATEGORY_PLAYERPROJECTILE;
	
}

#endif