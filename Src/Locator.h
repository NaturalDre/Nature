#pragma once

namespace VE
{
	class IRenderService;
	class IPhysicsService;
	class IGameManager;
	class CLocator
	{
	public:
		static void Provide(IRenderService* service) { m_renderService = service; }
		static void Provide (IPhysicsService* service) { m_physicsService = service; }
		static void Provide(IGameManager* gameManager) { m_gameManager = gameManager; }

		static IRenderService* GetRenderer(void) { return m_renderService; }
		static IPhysicsService* GetPhysics(void) { return m_physicsService; }
		static IGameManager* GetGameManager(void) { return m_gameManager; }
	private:
		static IRenderService* m_renderService;
		static IPhysicsService* m_physicsService;
		static IGameManager* m_gameManager;
	};
}