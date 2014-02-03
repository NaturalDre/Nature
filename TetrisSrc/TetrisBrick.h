#pragma once

#include <vector>

namespace Tetris
{
	class CTetris;
	class CTetrisBrick
	{
	public:
		typedef std::vector<std::vector<int>> Brick2D;

		CTetrisBrick(CTetris* tetrisBoard, size_t rows, size_t cols, size_t rotations);
		virtual ~CTetrisBrick(void);

		CTetris* GetBoard(void) { return m_board; }

		int GetCols(void) const { if (!m_brick.empty()) return m_brick.at(0).size(); else return 0; }
		int GetRows(void) const { if (!m_brick.empty()) return m_brick.size(); else return 0;}
		int GetValue(size_t row, size_t col);
		int GetRotationValue(size_t rotation, size_t row, size_t col);
		size_t GetCurRotation(void) { return m_curRotation; }
		void SetCurRotation(size_t rotation);	
		size_t GetRotations(void) { return m_rotations.size(); }
		inline int GetCurCol(void) const { return m_curCol; }
		inline int GetCurRow(void) const { return m_curRow; }
		inline void SetCurCol(int col) { m_curCol = col; }
		inline void SetCurRow(int row) { m_curRow = row; }

		inline bool CanKick(void) const { return m_canKick; }
		inline void CanKick(bool canKick) { m_canKick = canKick; }

		void ModifyRotation(size_t rotation, size_t row, size_t col, size_t value);

	protected:



	private:
		CTetris* m_board;
		Brick2D m_brick;
		std::vector<Brick2D> m_rotations;
		int m_curCol;
		int m_curRow;
		int m_curRotation;
		bool m_canKick;
	};
}