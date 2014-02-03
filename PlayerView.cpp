#include "PlayerView.h"
#include "Player.h"
#include <iostream>
#include <allegro5\allegro5.h>
#include "PlayerEvents.h"
#include "Draw.h"
#include <Bitmap.hpp>
#include "Locator.h"
#include "Render.h"

namespace VE
{
	CPlayerView::CPlayerView(CPlayer* player)
		: IView()
		, m_player(nullptr)
		, m_rc(static_cast<CRender*>(CLocator::GetRenderer()))
	{
		// CPlayer calls this constructor. SetPlayer calls one of CPlayer's functions before CPlayer
		// is fully constructed. Fix this.
		SetPlayer(player);
		m_anims.SetSpriteSheet("Images/player.png", 4, 3, 5); 
		m_anims.AddFrameSequence(FrameSequence("Walk_Down", 1, 3));
		m_anims.AddFrameSequence(FrameSequence("Walk_Left", 4, 6));
		m_anims.AddFrameSequence(FrameSequence("Walk_Up", 10, 12));
		m_anims.AddFrameSequence(FrameSequence("Walk_Right", 7, 9));

		m_anims.AddFrameSequence(FrameSequence("Idle_Down", 1,1));
		m_anims.AddFrameSequence(FrameSequence("Idle_Left", 4,4));
		m_anims.AddFrameSequence(FrameSequence("Idle_Up", 10,10));
		m_anims.AddFrameSequence(FrameSequence("Idle_Right", 7, 7));

		ChangeDirection(m_player->GetDirection());
	}

	CPlayerView::~CPlayerView(void)
	{
		if (m_player)
			m_player->Deregister(this);
		m_player = nullptr;
	}

	void CPlayerView::Draw(void)
	{

		if (!m_player)
			return;

		if (m_anims.GetCurrentAnim())
			VE::DrawBitmap(m_anims.GetCurrentAnim()->GetFrame(), m_player->GetPosition(), b2Vec2(0,0));
	}

	void CPlayerView::Notify(int eventType)
	{
		if (eventType == ALLEGRO_GET_EVENT_TYPE('D', 'I', 'R', 'C'))
			ChangeDirection(m_player->GetDirection());
		else if(eventType == ALLEGRO_GET_EVENT_TYPE('S', 'P', 'D', 'C'))
			OnSpeedChange();
	}

	void CPlayerView::ChangeDirection(DIRECTION dir)
	{
		const bool isIdle = m_player->GetSpeed() == b2Vec2(0,0);
		if (isIdle)
			int f(4);
		if (dir == RIGHT)
		{
			if (!isIdle)
				m_anims.SetCurrentAnim("Walk_Right");
			else 
				m_anims.SetCurrentAnim("Idle_Right");
		}
		else if (dir == LEFT)
		{
			if (!isIdle)
				m_anims.SetCurrentAnim("Walk_Left");
			else
				m_anims.SetCurrentAnim("Idle_Left");
		}
		else if (dir == UP)
		{
			if (!isIdle)
				m_anims.SetCurrentAnim("Walk_Up");
			else
				m_anims.SetCurrentAnim("Idle_Up");
		}
		else if (dir == DOWN)
		{
			if (!isIdle)
				m_anims.SetCurrentAnim("Walk_Down");
			else
				m_anims.SetCurrentAnim("Idle_Down");
		}
	}

	void CPlayerView::OnSpeedChange(void)
	{
		ChangeDirection(m_player->GetDirection());
	}

	void CPlayerView::SetPlayer(CPlayer* player)
	{
		if (m_player)
			m_player->Deregister(this);

		m_player = player;

		if (m_player)
		{
			ChangeDirection(m_player->GetDirection());
			m_player->Register(ALLEGRO_GET_EVENT_TYPE('S', 'P', 'D', 'C'), this);
			m_player->Register(ALLEGRO_GET_EVENT_TYPE('D', 'I', 'R', 'C'), this);
		}
	}
}