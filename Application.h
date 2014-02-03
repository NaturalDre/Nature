#ifndef APPLICATION_H
#define APPLICATION_H

#include "ErrorLogger.h"

struct lua_State;

namespace VE 
{
	class CEngine;
	class CGameMap;
	class CPlayer;
	//class CGameLevel;
	//class CGameLevelController;
	class CApplication
	{
	protected:
		void Init(void);
		void SetupDirectories(void);
		void PreLogic(void);
	public:
		CApplication(void);
		~CApplication(void);

		int Run(void);
	private:
		CEngine* m_engine;
		CGameMap* m_gameMap;
		CPlayer* m_player;
	};
}

#endif