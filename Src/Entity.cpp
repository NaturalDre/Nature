#include "Entity.h"
#include "GameManager.h"
#include "Component.h"
#include <lua.hpp>

namespace VE
{
	size_t IEntity::m_count = 0;

	IEntity::IEntity(IGameManager* gameManager)
		: m_gameManager(gameManager) 
		, m_enabled(true)
		, m_entityID(++m_count)
	{
		if (gameManager)
			gameManager->AddEntity(this);
	}

	IEntity::~IEntity(void)
	{
		if (m_gameManager)
			m_gameManager->RemoveEntity(this);
	}

	void IEntity::Update(double dt)
	{
		for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
			(*iter)->Update(dt);
	}
}