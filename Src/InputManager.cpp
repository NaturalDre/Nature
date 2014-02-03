#include "InputManager.h"
#include "InputController.h"
#include <allegro5\allegro5.h>
namespace VE
{
	//struct Key
	//{
	//	bool down;
	//	bool repeating;
	//};
	CInputManager* CInputManager::instance = nullptr;

	CInputManager::CInputManager(void)
	{

	}

	CInputManager::~CInputManager(void)
	{

	}

	void CInputManager::Update(void)
	{
		while (!m_events.empty())
		{
			ALLEGRO_EVENT ev = m_events.front();
			for(auto iter = m_controllers.begin(); iter != m_controllers.end(); ++iter)
				(*iter)->HandleEvent(ev);
			m_events.pop();
		}
	}

	CInputManager* CInputManager::Instance(void)
	{
		if (!instance)
			instance = new CInputManager;
			return instance;
	}
}