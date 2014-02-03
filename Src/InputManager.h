#pragma once

#include <list>
#include <queue>
#include <allegro5\allegro5.h>
namespace VE
{
	class IInputController;
	class CInputManager
	{

	public:
		CInputManager(void);
		~CInputManager(void);

		void Update(void);
		void PushEvent(const ALLEGRO_EVENT& ev) { m_events.push(ev); }
		void Add(IInputController* c) { m_controllers.push_back(c); }
		void Remove(IInputController* c) { m_controllers.remove(c); }

		static CInputManager* Instance(void);
	private:
		std::list<IInputController*> m_controllers;
		std::queue<ALLEGRO_EVENT> m_events;

		static CInputManager* instance;
	};
}