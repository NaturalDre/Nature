#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "View.h"
#include "Observer.h"
#include "MultiAnimation.h"
#include "Common.h"
#include "RenderComponent.h"

namespace VE
{
	class CPlayer;
	class IRenderService;
	class CPlayerView: public IView, public IObserver
	{
	protected:
		void Draw(void);
		void Notify(int eventCode);
		void ChangeDirection(DIRECTION dir);

		void OnSpeedChange(void);
	public:
		CPlayerView(CPlayer* player);
		~CPlayerView(void);

		CPlayer* Player(void) const { return m_player; }
		void SetPlayer(CPlayer* player);

	private:
		CPlayer* m_player;
		CMultiAnimation m_anims;
		CRenderComponentRotated m_rc;
	};
}
#endif