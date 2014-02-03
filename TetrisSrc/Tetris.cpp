#include "Tetris.h"
#include "Locator.h"
#include "TetrisView.h"
#include "TetrisController.h"
#include <allegro5\allegro.h>
#include "TetrisBrick.h"
#include <iostream>


namespace Tetris
{
	CTetrisBrick* CreateLBrick(CTetris*);
	CTetrisBrick* CreateJBrick(CTetris*);
	CTetrisBrick* CreateZBrick(CTetris*);
	CTetrisBrick* CreateZ2Brick(CTetris*);
	CTetrisBrick* CreateOBrick(CTetris*);
	CTetrisBrick* CreateTBrick(CTetris*);
	CTetrisBrick* CreateIBrick(CTetris*);



	const float GRAVITYTIME = 1.0f;

	int FindLowestC(CTetrisBrick* brick, size_t rotation)
	{
		assert(brick != nullptr);
		for (int c = 0; c < brick->GetCols(); ++c)
		{
			for (int r = 0; r < brick->GetRows(); ++r)
			{
				if (brick->GetRotationValue(rotation, r, c))
					return c;
			}
		}
		// This should NEVER be called unless the rotation doesn't exist or all it's values are 0.
		throw(std::exception("Tetris.cpp - FindLowestC - The rotation is not valid."));
	}

	int FindLowestR(CTetrisBrick* brick, size_t rotation)
	{
		assert(brick != nullptr);
		for (int r = 0; r < brick->GetRows(); ++r)
		{
			for (int c = 0; c < brick->GetCols(); ++c)
			{
				if (brick->GetRotationValue(rotation, r, c))
					return r;
			}
		}

		// This should NEVER be called unless the rotation doesn't exist or all it's values are 0.
		throw(std::exception("Tetris.cpp - FindLowestR - The rotation is not valid."));
	}

	int FindHighestR(CTetrisBrick* brick, int rotation)
	{	
		if (!brick)
			throw(std::exception("Tetris.cpp - FindHighestR - Brick is nullptr"));

		for (int r = brick->GetRows(); r >= 0; --r)
		{
			for (int c = brick->GetCols(); c >= 0; --c)
			{
				if (brick->GetRotationValue(rotation, r, c))
					return r;
			}
		}

		// This should NEVER be called unless the rotation doesn't exist or all it's values are 0.
		throw(std::exception("Tetris.cpp - FindHighestR - The rotation is not valid."));
	}

	int FindHighestC(CTetrisBrick* brick, int rotation)
	{
		if (!brick)
			throw(std::exception("Tetris.cpp - FindHighestC - Brick is nullptr"));

		for (int c = brick->GetCols(); c >= 0; --c)
		{
			for (int r = brick->GetRows(); r >= 0; --r)
			{
				if (brick->GetRotationValue(rotation, r, c))
					return c;
			}
		}

		// This should NEVER be called unless the rotation doesn't exist or all it's values are 0.
		throw(std::exception("Tetris.cpp - FindHighestC - The rotation is not valid."));
	}

	CTetris::CTetris(void)
		: VE::IEntity(VE::CLocator::GetGameManager())
		, m_gameState(MainMenu)
		, m_view(nullptr)
		, m_controller(nullptr)
		, m_curBrick(nullptr)
		, m_nextBrick(nullptr)
		, m_menu(nullptr)
		, m_offsetX(8)
		, m_offsetY(8)
		, m_timeSinceGravity(0.0f)
		, m_points(0)
		, m_gameInProgress(false)
		, m_paused(false)
		, m_forceGravity(false)
		, m_forceLock(false)
		, m_gameOver(false)

	{
		m_board = std::vector<std::vector<int>>(22, std::vector<int>(10, 0));
		m_view = new CTetrisView(this);
		m_controller = new CTetrisController(this);

		m_gameMusicSample.load("Sounds/game_music.ogg");
		m_dropSound = alx::Sample("Sounds/drop.ogg");
		m_clearSound = alx::Sample("Sounds/clear.flac");

		m_gameMusicInstance = alx::SampleInstance(m_gameMusicSample);
		m_gameMusicInstance.setPlayMode(ALLEGRO_PLAYMODE_LOOP);		

		al_attach_sample_instance_to_mixer(m_gameMusicInstance.get(), al_get_default_mixer());
	}


	CTetris::~CTetris(void)
	{
	}

	void CTetris::StartNewGame(void)
	{
		if (GetGameState() == GameRunning || GetGameState() == GamePaused)
			EndGame(false);
		m_curBrick = CreateRandomBrick();
		if (!m_gameMusicInstance.isPlaying())
				m_gameMusicInstance.play();
		SetGameState(GameRunning);
	}

	void CTetris::EndGame(bool stopMusic)
	{
		if (GetGameState() == GameRunning || GetGameState() == GamePaused)
		{
			delete m_curBrick;
			m_curBrick = nullptr;
			m_board = std::vector<std::vector<int>>(22, std::vector<int>(10, 0));
			SetTimeSinceGravity(0.0f);
			m_points = 0;
			if (stopMusic)
				m_gameMusicInstance.stop();
			SetGameState(MainMenu);
		}
	}

	void CTetris::SetGameOver(void)
	{
		EndGame();
		SetGameState(GameOver);
		if (m_gameMusicInstance.isPlaying())
			m_gameMusicInstance.stop();
	}

	void CTetris::PauseGame(void)
	{
		if (GetGameState() == GameRunning)
		{
			SetGameState(GamePaused);
			m_gameMusicInstance.stop();
		}
	}

	void CTetris::UnpauseGame(void)
	{
		if (GetGameState() == GamePaused)
		{
			SetGameState(GameRunning);
			m_gameMusicInstance.play();
		}
	}

	void CTetris::Update(double dt)
	{
		VE::IEntity::Update(dt);
		
		if (GetGameState() == GameRunning)
			ProcessGameRunning(dt);
	}

	void CTetris::ProcessMainMenu(void)
	{

	}

	void CTetris::ProcessGameRunning(double dt)
	{
		m_timeSinceGravity += dt;

		if (m_timeSinceGravity < GRAVITYTIME)
		{
			if (!m_forceGravity)
				return;
			else if (m_timeSinceGravity < (GRAVITYTIME / 20.0f))
				return;
		}


		if (LockCheck())
		{
			LockCurBrick();
			delete m_curBrick;
			m_curBrick = nullptr;
		}
		else
			ApplyGravity();

		if (!GetCurBrick())
		{
			delete m_curBrick;
			m_curBrick = nullptr;
			CheckDeleteFilledRows();
			m_curBrick = CreateRandomBrick();
			if (IsFail())
			{
				EndGame();
				SetGameOver();
			}
		}

		// Even if gravity wasn't applied, we set it to 0,
		// because a new brick would been created if Gravity never
		// got applied, and we want the timer to start over.
		m_timeSinceGravity = 0;
	}

	void CTetris::MoveLeft(void)
	{
		if (GetCurBrick() && IsLegalHMove(-1))
			GetCurBrick()->SetCurCol(GetCurBrick()->GetCurCol() - 1);
	}

	void CTetris::MoveRight(void)
	{
		// TODO: prevent possibility of moving any part of the brick off the right edge
		if (GetCurBrick() && IsLegalHMove(1))
			GetCurBrick()->SetCurCol(GetCurBrick()->GetCurCol() + 1);
	}

	bool CTetris::CanCurBrickRotate(int rotation, int& offsetCol)
	{
		if (!GetCurBrick())
			return false;

		//CTetrisBrick* curBrick = GetCurBrick();

		bool basic = true;
		bool oneLeft = true;
		bool oneRight = true;

		for (int r = 0; r < GetCurBrick()->GetRows(); ++r)
		{
			for (int c = 0; c < GetCurBrick()->GetCols(); ++c)
			{
				if (basic && GetCurBrick()->GetRotationValue(rotation, r,c) && GetValue(GetCurBrick()->GetCurRow() + r, GetCurBrick()->GetCurCol() + c))
					basic = false;
				if (oneRight && GetCurBrick()->GetRotationValue(rotation, r,c) && GetValue(GetCurBrick()->GetCurRow() + r, (GetCurBrick()->GetCurCol() + c) + 1))
					oneRight = false;
				if (oneLeft && GetCurBrick()->GetRotationValue(rotation, r,c) && GetValue(GetCurBrick()->GetCurRow() + r, (GetCurBrick()->GetCurCol() + c) - 1))
					oneLeft = false;

				if (!basic && !oneRight && !oneLeft)
					return false;
			}
		}

		int lc = FindLowestC(GetCurBrick(), rotation);
		int lr = FindLowestR(GetCurBrick(), rotation);
		int hr = FindHighestR(GetCurBrick(), rotation);
		int hc = FindHighestC(GetCurBrick(), rotation);


		if (GetCurBrick()->GetCurCol() + lc < 0)
			return false;
		if (GetCurBrick()->GetCurRow() + lr < 0)
			return false;
		if (GetCurBrick()->GetCurRow() + hr >= static_cast<int>(GetBoard().size()))
			return false;
		if (GetCurBrick()->GetCurCol() + hc >= static_cast<int>(GetBoard().at(0).size()))
			return false;

		if (basic)
		{
			offsetCol = 0;
			return basic;
		}
		else if (oneRight && GetCurBrick()->CanKick())
		{
			offsetCol = 1;
			return oneRight;
		}
		else if (oneLeft && GetCurBrick()->CanKick())
		{
			offsetCol = -1;
			return oneLeft;
		}
		return false;
	}

	void CTetris::RotateRight(void)
	{
		if (!GetCurBrick())
			return;

		CTetrisBrick* curBrick = GetCurBrick();

		size_t rotation = 0;
		if (curBrick->GetCurRotation() < (curBrick->GetRotations() - 1))
			rotation = curBrick->GetCurRotation() + 1;

		int offsetX = 0;


		if (CanCurBrickRotate(rotation, offsetX))
		{
			curBrick->SetCurRotation(rotation);
			curBrick->SetCurCol(curBrick->GetCurCol() + offsetX);
		}
	}

	void CTetris::ApplyGravity(void)
	{
		if (GetCurBrick())
			m_curBrick->SetCurRow(m_curBrick->GetCurRow() + 1);
	}

	bool CTetris::LockCheck(void)
	{
		if (!m_curBrick)
			return false;
		for (int r = GetCurBrick()->GetRows() - 1; r >= 0; --r)
		{
			for (int c = GetCurBrick()->GetCols() - 1; c >= 0; --c)
			{
				if (m_curBrick->GetValue(r,c))
				{
					int dc = GetCurBrick()->GetCurCol() + c;
					// The +1 is because we want to check if we'll overlap
					// with any bricks below the current brick when gravity
					/// is applied.
					int dr = GetCurBrick()->GetCurRow() + r + 1;
					// We also don't want to move any parts of 
					// the current brick below the last row.
					if (GetValue(dr, dc) || dr == m_board.size())
						return true;
				}
			}
		}
		return false;
	}

	// We'll lock each piece of the current brick
	// in the spot it is current at.
	void CTetris::LockCurBrick(void)
	{
		for (int r = 0; r < GetCurBrick()->GetRows(); ++r)
		{
			for (int c = 0; c < GetCurBrick()->GetCols(); ++c)
			{
				if (GetCurBrick()->GetValue(r,c))
					SetValue(GetCurBrick()->GetCurRow() + r, GetCurBrick()->GetCurCol() + c, GetCurBrick()->GetValue(r,c));
			}
		}
	}

	void CTetris::HardDrop(void)
	{
		if (GetGameState() != GameRunning)
			return;

		for (size_t r = 0; r < m_board.size(); ++r)
		{
			if (LockCheck())
			{
				LockCurBrick();
				Update(GRAVITYTIME);
				m_dropSound.play(1,1,1,ALLEGRO_PLAYMODE_ONCE);
				break;
			}
			GetCurBrick()->SetCurRow(GetCurBrick()->GetCurRow() + 1);
		}
	}

	int CTetris::GetValue(size_t r, size_t c) const
	{
		if (r < m_board.size() && c < m_board.at(r).size())
			return m_board[r][c];
		else 
			return 0;
	}

	void CTetris::SetValue(size_t r, size_t c, int value)
	{
		if (r < m_board.size() && c < m_board.at(r).size())
			m_board[r][c] = value;
		return;
	}

	bool CTetris::IsLegalHMove(int dir)
	{
		if (!GetCurBrick())
			return false;

		int highestC = 0;
		int lowestC = GetCurBrick()->GetCols() - 1;

		for (int r = 0; r < GetCurBrick()->GetRows(); ++r)
		{
			for (int c = 0; c < GetCurBrick()->GetRows(); ++c)
			{
				if (GetCurBrick()->GetValue(r,c))
				{
					if (c < lowestC)
						lowestC = c;
					if (c > highestC)
						highestC = c;
					if (GetValue(GetCurBrick()->GetCurRow() + r, (GetCurBrick()->GetCurCol() + c)+dir))
						return false;
				}
			}
		}
		// Check if we're moving part of the brick off the left edge.
		if ((dir < 0) && (GetCurBrick()->GetCurCol() + lowestC) < 1)
			return false;
		// Check if we're moving part of the brick off the right edge.
		if ((dir > 0) && (GetCurBrick()->GetCurCol() + highestC) >= static_cast<int>(m_board.at(0).size()) - 1)
			return false;
		return true;
	}

	CTetrisBrick* CTetris::CreateRandomBrick(void)
	{
		int r = rand() % 7 + 1;
		CTetrisBrick* brick = nullptr;

		switch(r)
		{
		case 1:
			brick = CreateLBrick(this);
			break;
		case 2:
			brick = CreateJBrick(this);
			break;
		case 3:
			brick = CreateZBrick(this);
			break;
		case 4:
			brick = CreateZ2Brick(this);
			break;
		case 5:
			brick = CreateOBrick(this);
			break;
		case 6:
			brick = CreateTBrick(this);
			break;
		case 7:
			brick = CreateIBrick(this);
			break;
		default:
			return nullptr;
			break;
		}

		int lr = FindLowestR(brick, brick->GetCurRotation());

		brick->SetCurRow(-1 * lr);
		return brick;
	}

	void CTetris::CheckDeleteFilledRows(void)
	{
		std::list<int> filledRows;
		int rowsCleared = 0;
		for (int r = 0; r < static_cast<int>(m_board.size()); ++r)
		{
			for (int c = 0; c < static_cast<int>(m_board.at(r).size()); ++c)
			{
				if (m_board.at(r).at(c) == 0)
					break;
				else if (c >= static_cast<int>(m_board.at(r).size()) - 1)
				{
					m_board.erase(m_board.begin() + r);
					m_board.insert(m_board.begin(), std::vector<int>(10, 0));
					filledRows.push_back(r);
					++rowsCleared;
				}
			}
		}

		m_points +=  m_board.at(0).size() * rowsCleared * rowsCleared;
		if (rowsCleared > 0)
			m_clearSound.play(1.5,1,1.5,ALLEGRO_PLAYMODE_ONCE);
	}

	bool CTetris::IsFail(void)
	{
		if (!GetCurBrick())
			return false;
		for (int r = GetCurBrick()->GetRows() - 1; r >= 0; --r)
		{
			for (int c = GetCurBrick()->GetCols() - 1; c >= 0; --c)
			{
				if (m_curBrick->GetValue(r,c))
				{
					int dc = GetCurBrick()->GetCurCol() + c;
					int dr = GetCurBrick()->GetCurRow() + r;
					int lr = FindLowestR(GetCurBrick(), GetCurBrick()->GetCurRotation());
					if (GetValue(dr, dc) || dr == m_board.size())
						return true;
				}
			}
		}
		return false;
	}


	CTetrisBrick* CreateLBrick(CTetris* tetris)
	{
		assert(tetris != nullptr);
		auto brick = new CTetrisBrick(tetris, 3, 3, 4);

		brick->ModifyRotation(0, 1, 0, 1);
		brick->ModifyRotation(0, 1, 1, 1);
		brick->ModifyRotation(0, 1, 2, 1);
		brick->ModifyRotation(0, 2, 0, 1);

		brick->ModifyRotation(1, 0, 0, 1);
		brick->ModifyRotation(1, 0, 1, 1);
		brick->ModifyRotation(1, 1, 1, 1);
		brick->ModifyRotation(1, 2, 1, 1);

		brick->ModifyRotation(2, 2, 0, 1);
		brick->ModifyRotation(2, 2, 1, 1);
		brick->ModifyRotation(2, 2, 2, 1);
		brick->ModifyRotation(2, 1, 2, 1);

		brick->ModifyRotation(3, 0, 1, 1);
		brick->ModifyRotation(3, 1, 1, 1);
		brick->ModifyRotation(3, 2, 1, 1);
		brick->ModifyRotation(3, 2, 2, 1);

		brick->SetCurRotation(0);
		return brick;
	}

	CTetrisBrick* CreateJBrick(CTetris* tetris)
	{
		assert(tetris != nullptr);
		auto brick = new CTetrisBrick(tetris, 3, 3, 4);

		brick->ModifyRotation(0, 1, 0, 2);
		brick->ModifyRotation(0, 1, 1, 2);
		brick->ModifyRotation(0, 1, 2, 2);
		brick->ModifyRotation(0, 2, 2, 2);

		brick->ModifyRotation(1, 0, 1, 2);
		brick->ModifyRotation(1, 1, 1, 2);
		brick->ModifyRotation(1, 2, 1, 2);
		brick->ModifyRotation(1, 2, 0, 2);

		brick->ModifyRotation(2, 1, 0, 2);
		brick->ModifyRotation(2, 2, 0, 2);
		brick->ModifyRotation(2, 2, 1, 2);
		brick->ModifyRotation(2, 2, 2, 2);

		brick->ModifyRotation(3, 0, 1, 2);
		brick->ModifyRotation(3, 0, 2, 2);
		brick->ModifyRotation(3, 1, 1, 2);
		brick->ModifyRotation(3, 2, 1, 2);


		brick->SetCurRotation(0);
		return brick;
	}

	CTetrisBrick* CreateZBrick(CTetris* tetris)
	{
		assert(tetris != nullptr);
		auto brick = new CTetrisBrick(tetris, 3, 3, 2);

		brick->ModifyRotation(0, 1, 1, 3);
		brick->ModifyRotation(0, 1, 2, 3);
		brick->ModifyRotation(0, 2, 0, 3);
		brick->ModifyRotation(0, 2, 1, 3);

		brick->ModifyRotation(1, 0, 0, 3);
		brick->ModifyRotation(1, 1, 0, 3);
		brick->ModifyRotation(1, 1, 1, 3);
		brick->ModifyRotation(1, 2, 1, 3);

		brick->SetCurRotation(0);
		return brick;
	}

	CTetrisBrick* CreateZ2Brick(CTetris* tetris)
	{
		assert(tetris != nullptr);
		auto brick = new CTetrisBrick(tetris, 3, 3, 2);

		brick->ModifyRotation(0, 1, 0, 4);
		brick->ModifyRotation(0, 1, 1, 4);
		brick->ModifyRotation(0, 2, 1, 4);
		brick->ModifyRotation(0, 2, 2, 4);

		brick->ModifyRotation(1, 0, 2, 4);
		brick->ModifyRotation(1, 1, 1, 4);
		brick->ModifyRotation(1, 1, 2, 4);
		brick->ModifyRotation(1, 2, 1, 4);

		brick->SetCurRotation(0);
		return brick;
	}

	CTetrisBrick* CreateOBrick(CTetris* tetris)
	{
		assert(tetris != nullptr);
		auto brick = new CTetrisBrick(tetris, 3, 3, 1);

		brick->ModifyRotation(0, 0, 0, 5);
		brick->ModifyRotation(0, 0, 1, 5);
		brick->ModifyRotation(0, 1, 0, 5);
		brick->ModifyRotation(0, 1, 1, 5);


		brick->SetCurRotation(0);
		return brick;
	}

	CTetrisBrick* CreateTBrick(CTetris* tetris)
	{
		assert(tetris != nullptr);
		auto brick = new CTetrisBrick(tetris, 3, 3, 4);

		brick->ModifyRotation(0, 1, 0, 6);
		brick->ModifyRotation(0, 1, 1, 6);
		brick->ModifyRotation(0, 1, 2, 6);
		brick->ModifyRotation(0, 2, 1, 6);

		brick->ModifyRotation(1, 0, 1, 6);
		brick->ModifyRotation(1, 1, 1, 6);
		brick->ModifyRotation(1, 2, 1, 6);
		brick->ModifyRotation(1, 1, 0, 6);

		brick->ModifyRotation(2, 1, 1, 6);
		brick->ModifyRotation(2, 2, 0, 6);
		brick->ModifyRotation(2, 2, 1, 6);
		brick->ModifyRotation(2, 2, 2, 6);

		brick->ModifyRotation(3, 0, 1, 6);
		brick->ModifyRotation(3, 1, 1, 6);
		brick->ModifyRotation(3, 2, 1, 6);
		brick->ModifyRotation(3, 1, 2, 6);


		brick->SetCurRotation(0);
		return brick;
	}

	CTetrisBrick* CreateIBrick(CTetris* tetris)
	{
		assert(tetris != nullptr);
		auto brick = new CTetrisBrick(tetris, 4, 4, 2);

		brick->ModifyRotation(0, 1, 0, 7);
		brick->ModifyRotation(0, 1, 1, 7);
		brick->ModifyRotation(0, 1, 2, 7);
		brick->ModifyRotation(0, 1, 3, 7);

		brick->ModifyRotation(1, 0, 2, 7);
		brick->ModifyRotation(1, 1, 2, 7);
		brick->ModifyRotation(1, 2, 2, 7);
		brick->ModifyRotation(1, 3, 2, 7);

		brick->SetCurRotation(0);
		brick->CanKick(false);
		return brick;
	}

}