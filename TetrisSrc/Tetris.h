#pragma once

#include "Entity.h"
#include <vector>
#include <SampleInstance.hpp>
#include <Voice.hpp>

namespace Tetris
{

	class CTetrisView;
	class CTetrisController;
	class CTetrisBrick;
	class CTetrisMenu;
	class CTetris: public VE::IEntity
	{
	public:
		enum State { MainMenu, GameRunning, GamePaused, GameOver };
		CTetris(void);
		~CTetris(void);

		inline const std::vector<std::vector<int>>& GetBoard(void) const { return m_board; }
		void MoveLeft(void);
		void MoveRight(void);
		inline void ForceGravity(bool force) { m_forceGravity = force; }
		void RotateRight(void);
		bool CanCurBrickRotate(int rotation, int& offsetCol);
		inline CTetrisBrick* GetCurBrick(void) const { return m_curBrick; }
		int GetValue(size_t r, size_t c) const;
		void HardDrop(void);
		inline int GetOffsetX(void) const { return m_offsetX; }
		inline int GetOffsetY(void) const { return m_offsetY; }
		inline int GetPoints(void) const { return m_points; }

		//inline bool IsPaused(void) const { return m_paused; }
		//inline bool IsGameInProgress(void) const { return m_gameInProgress; }
		//inline bool IsGameOver(void) const { return m_gameOver; }

		void StartNewGame(void);
		/// @param failed True if we're ending the game due to player losing(Game over). False otherwise.
		void EndGame(bool stopMusic = true);
		void PauseGame(void);
		void UnpauseGame(void);

		State GetGameState(void) const { return m_gameState; }

	protected:
		void Update(double dt);
		void ApplyGravity(void);
		// Check if the currently falling brick needs to be locked in place.
		bool LockCheck(void);
		// Lock the currently falling piece in place and create a new one.
		void LockCurBrick(void);
		// Check to see if a row is filled; if so, delete it.
		void CheckDeleteFilledRows(void);

		void SetValue(size_t r, size_t c, int value);
		// Should it be possible to move to the left or right?
		bool IsLegalHMove(int dir);
		CTetrisBrick* CreateRandomBrick(void);

		bool IsFail(void);
		void SetGameOver(void);

		inline void SetTimeSinceGravity(float seconds) { m_timeSinceGravity = seconds; }
		//inline void SetPaused(bool paused) { m_paused = paused; }
		//inline void SetGameInProgress(bool inProgress) { m_gameInProgress = inProgress; }
		//inline void SetGameOver(bool gameOver) { m_gameOver = gameOver; }

		void SetGameState(State state) { m_gameState = state; }
		void ProcessMainMenu(void);
		void ProcessGameRunning(double dt);
	private:
		std::vector<std::vector<int>> m_board;
		State m_gameState;
		CTetrisView* m_view;
		CTetrisController* m_controller;
		CTetrisBrick* m_curBrick;
		CTetrisBrick* m_nextBrick;
		CTetrisMenu* m_menu;	
		alx::SampleInstance m_gameMusicInstance;
		alx::Sample m_gameMusicSample;
		alx::SampleId m_gameMusicID;
		alx::Sample m_dropSound;
		alx::Sample m_clearSound;
		//alx::Voice m_voice;
		int m_offsetX;
		int m_offsetY;
		double m_timeSinceGravity;
		int m_points;
		// Is a game currently in progress? This doesn't mean the game is paused/unpaused.
		bool m_gameInProgress;
		bool m_paused;
		bool m_forceGravity;
		bool m_forceLock;
		bool m_gameOver;
	};

}