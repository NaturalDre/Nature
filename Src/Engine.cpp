#include "Engine.h"
#include "Vengine.h"

#include <allegro5\allegro5.h>
#include <allegro5\allegro_physfs.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <algorithm>
#include <iostream>
#include <physfs.h>
#include <lua.hpp>
#include <lauxlib.h>
#include "Physics.h"
#include "Render.h"
#include "InputManager.h"
#include "Utility.h"
#include "Locator.h"
#include "GameManager.h"
#include "GameMap.h"
#include "Entity.h"


namespace VE
{
	CEngine::CEngine(void)
		: m_timer(nullptr)
		, m_evQ(nullptr)
		, m_display(nullptr)
		, m_physics(nullptr)
		, m_renderer(nullptr)
		, m_gameManager(nullptr)
		, m_console(nullptr)
		, m_done(false)
		, m_isInit(false)
		, m_pausePhysics(true)
		, m_pauseLogic(true)
		, m_needRedraw(true)
		, m_timeLastUpdated(0)
		, m_gameTime(0)
	{

	}

	CEngine::~CEngine(void)
	{
		// We don't own it.
		m_callback = nullptr;

		al_destroy_display(m_display);
		m_display = nullptr;

		al_destroy_timer(m_timer);
		m_timer = nullptr;

		al_destroy_event_queue(m_evQ);
		m_evQ = nullptr;
	}

	int CEngine::InitBase(void)
	{
		if (!al_init())
			return -1;

		if (!al_init_image_addon())
			return -2;

		if (!al_init_primitives_addon())
			return -3;

		if (!al_install_keyboard())
			return -4;

		if (!al_install_mouse())
			return -5;

		al_init_font_addon();

		if (!al_init_ttf_addon())
			return -7;

		if (!al_install_audio())
			return -8;
		if (!al_init_acodec_addon())
			return -9;
		if (!al_reserve_samples(10))
			return -10;

		return 0;
	}

	int CEngine::Init(void)
	{
		try
		{
			if (InitBase())
				return -1;

			PHYSFS_init(nullptr);
			PHYSFS_addToSearchPath("Core", 1);
			PHYSFS_addToSearchPath("Data", 1);
			al_set_physfs_file_interface();	

			m_evQ = al_create_event_queue();
			m_timer = al_create_timer(1.0f / 60.0f);
			m_display = al_create_display(800, 720);
			m_gameManager = new CGameManager(this);
			CLocator::Provide(m_gameManager);
			m_renderer = new CRender;
			CLocator::Provide(m_renderer);
			m_physics = new CPhysics;
			CLocator::Provide(m_physics);

			al_register_event_source(m_evQ, al_get_timer_event_source(m_timer));
			al_register_event_source(m_evQ, al_get_display_event_source(m_display));
			al_register_event_source(m_evQ, al_get_keyboard_event_source());
			al_register_event_source(m_evQ, al_get_mouse_event_source());

			m_isInit = true;
		}
		catch(...)
		{
			m_isInit = false;
			return 1;
		}

		return 0;
	}

	int CEngine::Run(boost::function<void()>& f)
	{
		if (!m_isInit)
			return -1;

		al_start_timer(m_timer);
		if(!f.empty())
			f();

		while(!m_done)
		{
			ALLEGRO_EVENT ev;
			al_wait_for_event(m_evQ, &ev);

			if (ev.type == ALLEGRO_EVENT_TIMER)
				Update();
			else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				break;
			else if (ev.type >= ALLEGRO_EVENT_JOYSTICK_AXIS && ev.type < ALLEGRO_EVENT_TIMER)
				HandleEvent(ev);

			if (m_needRedraw && al_is_event_queue_empty(m_evQ))
				Render();
		}

		return 0;
	}

	void CEngine::Update(void)
	{
		const double ct = al_current_time();
		const double dt = ct - m_timeLastUpdated;
		m_timeLastUpdated = ct;

		if (!IsLogicPaused())
		{
			m_gameTime += dt;
			CInputManager::Instance()->Update();
			for (auto iter = m_gameManager->m_entities.begin(); iter != m_gameManager->m_entities.end(); ++iter)
			{
				if ((*iter)->IsEnabled())
					(*iter)->Update(dt);
			}
		}

		if (!IsPhysicsPaused())
			GetPhysics()->Simulate();
	}

	void CEngine::Render(void)
	{
		CLocator::GetRenderer()->Render();
	}

	void CEngine::HandleEvent(ALLEGRO_EVENT& ev)
	{
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_TILDE)
		{
			SetPauseLogic(!IsLogicPaused());
			SetPausePhysics(!IsPhysicsPaused());
		}
		CInputManager::Instance()->PushEvent(ev);
	}
}