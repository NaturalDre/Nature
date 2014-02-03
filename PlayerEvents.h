#ifndef PLAYEREVENTS_H
#define PLAYEREVENTS_H

#include "Event.h"
#include "Common.h"
#include <allegro5\allegro5.h>

namespace VE
{
	class DirectionChanged: public IEvent
	{
	private:

	public:
		DirectionChanged(DIRECTION dir): IEvent(ALLEGRO_GET_EVENT_TYPE('D', 'I', 'R', 'C')), m_dir(dir) { }
		DIRECTION GetDir(void) const { return m_dir; }
	private:
		DIRECTION m_dir;
	};
};

#endif