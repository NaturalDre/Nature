#include "PlayerController.h"
#include "Player.h"
#include "PlayerView.h"
#include <assert.h>
#include <allegro5\allegro5.h>
#include "Render.h"
#include "Locator.h"

namespace VE
{
	CPlayerController::CPlayerController(CPlayer* player)
		: m_player(player)
		, m_movedLThisFrame(false)
		, m_movedRThisFrame(false)
		, m_movedUpThisFrame(false)
		, m_movedDownThisFrame(false)
	{
		m_keys.resize(ALLEGRO_KEY_MAX, false);
	}

	CPlayerController::~CPlayerController(void)
	{
		m_player = nullptr;
	}

	void CPlayerController::HandleEvent(const ALLEGRO_EVENT& ev)
	{
		if (!m_player)
			return;

		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			HandleMouseMove(ev);
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			m_keys[ev.keyboard.keycode] = true;
			HandleKeyDown(ev);
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			m_keys[ev.keyboard.keycode] = false;
			HandleKeyUp(ev);
		}
	}

	void CPlayerController::HandleKeyDown(const ALLEGRO_EVENT& ev)
	{
		m_keys[ev.keyboard.keycode] = true;
	}

	void CPlayerController::HandleKeyUp(const ALLEGRO_EVENT& ev)
	{
		m_keys[ev.keyboard.keycode] = false;
	}

	void CPlayerController::ProcessKeys(void)
	{
		DIRECTION tDir = m_player->GetDirection();

		b2Vec2 speed(0,0);
		if (IsKeyDown(ALLEGRO_KEY_D) && !IsKeyDown(ALLEGRO_KEY_A))
		{
			speed.x += 5;
			tDir = RIGHT;
		}
		else if (IsKeyDown(ALLEGRO_KEY_A) && !IsKeyDown(ALLEGRO_KEY_D))
		{
			speed.x -= 5;
			tDir = LEFT;
		}
		if (IsKeyDown(ALLEGRO_KEY_W) && !IsKeyDown(ALLEGRO_KEY_S))
		{
			speed.y -= 5;
			tDir = UP;
		}
		else if (IsKeyDown(ALLEGRO_KEY_S) && !IsKeyDown(ALLEGRO_KEY_W))
		{
			speed.y += 5;
			tDir = DOWN;
		}

		m_player->SetSpeed(speed);
		m_player->SetDirection(tDir);
	}

	void CPlayerController::HandleMouseMove(const ALLEGRO_EVENT& ev)
	{
		m_mousePos = CLocator::GetRenderer()->GetCamera()->TopLeftPosPix() + b2Vec2(ev.mouse.x, ev.mouse.y);
		m_mousePos = PixToMtr(m_mousePos);
	}

	void CPlayerController::Update(double deltaTime)
	{
		m_movedLThisFrame = false;
		m_movedRThisFrame = false;
		m_movedUpThisFrame = false;
		m_movedDownThisFrame = false;

		if (!m_player)
			return;

		ProcessKeys();
	}

	void CPlayerController::SetPlayer(CPlayer* player)
	{
		m_player = player;
	}
	bool CPlayerController::IsKeyDown(size_t key) 
	{ 
		return m_keys[key];
	}
}