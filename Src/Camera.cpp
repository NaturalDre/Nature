#include "Camera.h"
#include "Player.h"

namespace VE
{
	CCamera::CCamera(CPlayer* player, size_t width, size_t height)
		: m_player(player)
		, m_width(width)
		, m_height(height)
	{


	}

	CCamera::~CCamera(void)
	{
		m_player = nullptr;
	}

	b2Vec2 CCamera::PosPix(void) const 
	{ 
		if (m_player) 
			return MtrToPix(m_player->GetPosition()); 
		return b2Vec2(0,0); 
	}

	b2Vec2 CCamera::PosMtr(void) const 
	{ 
		if (m_player) 
			return m_player->GetPosition(); 
		return b2Vec2(0,0); 
	}
}