#pragma once

#include "InputController.h"

namespace Tetris
{
	class CTetris;
	class CTetrisController: public VE::IInputController
	{
	public:
		CTetrisController(CTetris* tetris);
		~CTetrisController(void);

	protected:	
		void HandleEvent(const ALLEGRO_EVENT& ev);

	private:
		CTetris* m_board;
		// Key cooldown
		double m_lastTimeKeyPressed;
	};
}