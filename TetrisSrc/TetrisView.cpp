#include "TetrisView.h"
#include "Tetris.h"
#include "TetrisBrick.h"

#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>


namespace Tetris
{
	const int SQUARE_SIZE = 32;

	const int LINECOLOR_R = 255;
	const int LINECOLOR_G = 255;
	const int LINECOLOR_B = 255;


	void CTetrisView::DrawString(const std::string& str, float dx, float dy)
	{
		al_draw_text(
			m_font,
			al_map_rgb(0, 255, 0),
			dx,
			dy,
			0,
			str.c_str());
	}

	ALLEGRO_COLOR GetColor(int id)
	{
		ALLEGRO_COLOR color = al_map_rgb(0,0,0);
		switch(id)
		{
		case 1:
			color = al_map_rgb(255,0,0);
			break;
		case 2: 
			color = al_map_rgb(0, 255, 0);
			break;
		case 3:
			color = al_map_rgb(0, 0, 255);
			break;
		case 4: 
			color = al_map_rgb(0, 255, 255);
			break;
		case 5:
			color = al_map_rgb(160, 32, 240);
			break;
		case 6:
			color = al_map_rgb(255, 165, 0);
			break;
		case 7:
			color = al_map_rgb(255, 255, 0);
			break;
		}
		return color;
	}

	CTetrisView::CTetrisView(CTetris* tetris)
		: m_tetris(tetris)
		, m_font(nullptr)
	{
		m_font = al_load_font("Fonts/OpenSans.ttf", 24, 0);
	}

	CTetrisView::~CTetrisView(void)
	{

	}

	void CTetrisView::Draw(void)
	{
		//const ALLEGRO_COLOR RED = al_map_rgb(255,0,0);
		//const ALLEGRO_COLOR GREEN = al_map_rgb(0, 255, 0);
		//const ALLEGRO_COLOR BLUE = al_map_rgb(0, 0, 255);
		//const ALLEGRO_COLOR CYAN = al_map_rgb(0, 255, 255);
		//const ALLEGRO_COLOR PURPLE  = al_map_rgb(160, 32, 240);
		//const ALLEGRO_COLOR ORANGE = al_map_rgb(255, 165, 0);
		//const ALLEGRO_COLOR YELLOW = al_map_rgb(255, 255, 0);


		auto& board = m_tetris->GetBoard();

		// Render locked pieces
		for (size_t i = 0; i < board.size(); ++i)
		{
			for (size_t j = 0; j < board.at(i).size(); ++j)
			{
				if (board.at(i).at(j) != 0)
				{
					float x1 = j * SQUARE_SIZE + m_tetris->GetOffsetX();
					float y1 = i * SQUARE_SIZE + m_tetris->GetOffsetY();
					float x2 = x1 + SQUARE_SIZE ;
					float y2 = y1 + SQUARE_SIZE;

					al_draw_filled_rectangle(x1, y1, x2, y2, GetColor(m_tetris->GetValue(i,j))); 
				}
			}
		}

		if (m_tetris->GetCurBrick())
		{

			// Render the current piece
			for (int i = 0; i < m_tetris->GetCurBrick()->GetCols(); ++i)
			{
				for (int j = 0; j < m_tetris->GetCurBrick()->GetRows(); ++j)
				{
					if (m_tetris->GetCurBrick()->GetValue(i, j) != 0)
					{
						// Find the local x1 (in reference to the entire brick)
						// of the square we're trying to draw.
						float x1 = j * SQUARE_SIZE;
						// We must find out the "world" position of the current square's x origin
						// and add the world position to it. This gives us the actual world x1
						// position of the current piece.
						x1 += m_tetris->GetCurBrick()->GetCurCol() * SQUARE_SIZE;
						// We do the same thing for y1;
						float y1 = i * SQUARE_SIZE;
						y1 += m_tetris->GetCurBrick()->GetCurRow() * SQUARE_SIZE;

						x1 += m_tetris->GetOffsetX();
						y1 += m_tetris->GetOffsetY();

						float x2 = x1 + SQUARE_SIZE;
						float y2 = y1 + SQUARE_SIZE;


						//ALLEGRO_COLOR color;
						const int value = m_tetris->GetCurBrick()->GetValue(i, j);
						al_draw_filled_rectangle(x1, y1, x2, y2, GetColor(value)); 
						al_draw_rectangle(x1,y1,x2,y2, al_map_rgb(255,255,255), -1);
					}
				}
			}
		}

		for (int i = 0; i < board.size() + 1; ++i)
		{
			// I use board.at(0) instead of board.at(i), because 
			// I want an extra line drawn at the bottom. If I used 'i',
			// it would crash with the '+1' in the above for loop.
			for (int j = 0; j < board.at(0).size() + 1; ++j)
			{
				{
					int x1 = m_tetris->GetOffsetX();
					int y1 = i * SQUARE_SIZE + m_tetris->GetOffsetY();;
					int x2 = board.at(0).size() * SQUARE_SIZE + m_tetris->GetOffsetX();
					int y2 = y1;



					al_draw_line(x1, y1, x2, y2, al_map_rgb(LINECOLOR_R,LINECOLOR_G,LINECOLOR_B), 1);
				}
				{
					int x1 = j * SQUARE_SIZE + m_tetris->GetOffsetX();;
					int y1 = m_tetris->GetOffsetY();
					int x2 = x1;
					int y2 = board.size() * SQUARE_SIZE + m_tetris->GetOffsetY();

					al_draw_line(x1, y1, x2, y2, al_map_rgb(LINECOLOR_R,LINECOLOR_G,LINECOLOR_B), 1);
				}
			}
		}

		DrawMenu();
	}

	void CTetrisView::DrawMenu(void)
	{
		float dx = 400;
		DrawString("Space == Hard Drop", dx, 32);
		DrawString("A/S/D == Move LEFT/DOWN/RIGHT", dx, 64);
		DrawString("R == Rotate", dx, 96);
		DrawString("N == New Game", dx, 128);
		DrawString("P == Pause Game", dx, 160);

		if (m_tetris->GetGameState() == CTetris::GamePaused)
			al_draw_text(
			m_font,
			al_map_rgb(255, 0, 0),
			dx,
			192,
			0,
			"Game Is Currently Paused");		
		else if(m_tetris->GetGameState() == CTetris::GameOver)
			al_draw_text(
			m_font,
			al_map_rgb(255, 0, 0),
			dx,
			192,
			0,
			"GAME OVER!");	

			al_draw_textf(
			m_font,
			al_map_rgb(0, 0, 255),
			dx,
			256,
			0,
			"Points: %d", m_tetris->GetPoints());
	}
}
