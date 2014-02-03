#include "TetrisBrick.h"
#include <assert.h>
#include "Tetris.h"

namespace Tetris
{
	CTetrisBrick::CTetrisBrick(CTetris* tetrisBoard, size_t rows, size_t cols, size_t rotations)
		: m_board(tetrisBoard)
		, m_brick(rows, std::vector<int>(cols, 0))
		, m_rotations(rotations, Brick2D(rows, std::vector<int>(cols, 0)))
		, m_curCol(0)
		, m_curRow(0)
		, m_curRotation(0)
		, m_canKick(true)
	{
		assert(m_board != nullptr);
	}

	CTetrisBrick::~CTetrisBrick(void)
	{
		m_board = nullptr;
	}

	void CTetrisBrick::ModifyRotation(size_t rotation, size_t row, size_t col, size_t value)
	{
		if ((rotation >= m_rotations.size()) || (row >= m_brick.size()) || (col >= m_brick.at(0).size()))
			return;
		m_rotations.at(rotation)[row][col] = value;
	}

	int CTetrisBrick::GetValue(size_t row, size_t col)
	{
		if ((m_curRotation >= m_rotations.size()) || (row >= static_cast<int>(m_brick.size())) || (col >= static_cast<int>(m_brick.at(0).size())))
			return 0;
		return m_rotations.at(m_curRotation)[row][col];
	}

	void CTetrisBrick::SetCurRotation(size_t rotation)
	{
		if (rotation >= m_rotations.size())
			m_curRotation = 0;
		else
			m_curRotation = rotation;
	}

	int CTetrisBrick::GetRotationValue(size_t rotation, size_t row, size_t col)
	{
		if ((rotation >= m_rotations.size()) || (row >= m_brick.size()) || (col >= m_brick.at(0).size()))
			return 0;
		return m_rotations.at(rotation)[row][col];
	}
}