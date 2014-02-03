#include "TetrisApp.h"
#include "Engine.h"
#include <physfs.h>
#include <allegro5\allegro_physfs.h>
#include "LuabindExport.h"
#include "Utility.h"
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include "GameMap.h"
#include "Locator.h"
#include "Render.h"
#include "Tetris.h"
#include "AssetManager.h"

namespace Tetris
{

	CTetrisApp::CTetrisApp(void)
		: m_engine(nullptr)
	{
		srand(time(nullptr));

		m_engine = new VE::CEngine;
		m_engine->Init();
	}

	CTetrisApp::~CTetrisApp(void)
	{
		delete m_engine;
		m_engine = nullptr;
	}

	void CTetrisApp::Init(void)
	{
		SetupDirectories();
	}

	void CTetrisApp::SetupDirectories(void)
	{
		PHYSFS_addToSearchPath("TetrisData", 1);
	}

	void CTetrisApp::PreLogic(void)
	{
		m_engine->SetPauseLogic(false);
	}

	int CTetrisApp::Run(void)
	{
		Init();
		CTetris t;
		boost::function<void()> f = boost::bind(&CTetrisApp::PreLogic, this);
		return m_engine->Run(f);
	}
}