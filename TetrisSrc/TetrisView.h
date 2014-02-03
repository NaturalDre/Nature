#pragma once

#include "View.h"
#include <string>

struct ALLEGRO_FONT;
namespace Tetris
{
	class CTetris;

	class CTetrisView: public VE::IView
	{
	public:
		CTetrisView(CTetris* tetris);
		~CTetrisView(void);

	protected:
		void Draw(void);
		void DrawMenu(void);
		void DrawString(const std::string& str, float dx, float dy);

	private:
		CTetris* m_tetris;
		ALLEGRO_FONT* m_font;
	};
}