#include "Application.h"
#include "Engine.h"
#include "GameManager.h"
#include <physfs.h>
#include <allegro5\allegro_physfs.h>
#include "LuabindExport.h"
#include "Utility.h"
#include "AssetManager.h"
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include "GameMap.h"
#include "Player.h"
#include "Locator.h"
#include "Render.h"


namespace VE
{
	CApplication::CApplication(void)
		: m_engine(nullptr)
		, m_gameMap(nullptr)
		, m_player(nullptr)	
	{
		srand(time(nullptr));

		m_engine = new CEngine;
		m_engine->Init();
	}

	CApplication::~CApplication(void)
	{
		delete m_engine;
		m_engine = nullptr;
	}

	void CApplication::Init(void)
	{
		SetupDirectories();
	}

	void CApplication::SetupDirectories(void)
	{
		PHYSFS_addToSearchPath("GameData", 1);
	}

	int CApplication::Run(void)
	{
		Init();
		CAssetManager::Instance()->LoadAssetsFromXML("Assets.xml");
		CAssetManager::Instance()->SetCurScene(1);
		m_gameMap = new CGameMap(m_engine->GetGameManager());
		boost::function<void()> f = boost::bind(&CApplication::PreLogic, this);
		return m_engine->Run(f);
	}

	void CApplication::PreLogic(void)
	{
		m_gameMap->Read("Maps/Adventure/Adventure.lua");
		m_player = CreatePlayer(m_engine->GetGameManager(), b2Vec2(2,2));
		CLocator::GetRenderer()->GetCamera()->Watch(m_player);
		m_engine->SetPauseLogic(false);
		m_engine->SetPausePhysics(false);
		//gm.Read("Maps/Adventure/Nature.lua");
	}
}