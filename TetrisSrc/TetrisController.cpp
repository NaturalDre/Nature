#include "TetrisController.h"
#include "allegro5\allegro.h"
#include "Tetris.h"
#include "TetrisBrick.h"
#include <iostream>

namespace Tetris
{

	CTetrisController::CTetrisController(CTetris* tetris)
		: m_board(tetris)
		, m_lastTimeKeyPressed(0.0f)
	{

	}

	CTetrisController::~CTetrisController(void)
	{

	}

	void CTetrisController::HandleEvent(const ALLEGRO_EVENT& ev)
	{
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			double curTime = al_current_time();

			if ((curTime - m_lastTimeKeyPressed) > 1.0f / 100.0f)
			{
				m_lastTimeKeyPressed = curTime;
				if (ev.keyboard.keycode == ALLEGRO_KEY_A)
					m_board->MoveLeft();
				else if (ev.keyboard.keycode == ALLEGRO_KEY_D)
					m_board->MoveRight();
				else if (ev.keyboard.keycode == ALLEGRO_KEY_S)
					m_board->ForceGravity(true);
				else if (ev.keyboard.keycode == ALLEGRO_KEY_R)
					m_board->RotateRight();
				else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
					m_board->HardDrop();
				else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_N)
					m_board->StartNewGame();
				else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_P)
				{
					if (m_board->GetGameState() == CTetris::GamePaused)
						m_board->UnpauseGame();
					else if (m_board->GetGameState() == CTetris::GameRunning)
						m_board->PauseGame();
				}
			}

		}

		if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			double curTime = al_current_time();
			{
				m_lastTimeKeyPressed = curTime;
				if (ev.keyboard.keycode == ALLEGRO_KEY_S)
					m_board->ForceGravity(false);

			}

		}
	}

}