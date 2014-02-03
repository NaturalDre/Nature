#include "Locator.h"

namespace VE
{
	IRenderService* CLocator::m_renderService = nullptr;
	IPhysicsService* CLocator::m_physicsService = nullptr;
	IGameManager* CLocator::m_gameManager = nullptr;
}