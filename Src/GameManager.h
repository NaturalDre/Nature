#pragma once

#include <unordered_set>

struct lua_State;
namespace VE
{
	class CEngine;
	class IEntity;
	class IAnimation;
	class IGameManager
	{
		friend CEngine;
	public:
		IGameManager(CEngine* engine): m_engine(engine) { }
		virtual ~IGameManager(void) = 0 { }
		virtual void AddAnimation(IAnimation* anim) { m_animations.insert(anim); }
		virtual void RemoveAnimation(IAnimation* anim) { m_animations.erase(anim); }

		virtual void AddEntity(IEntity* entity) { m_entities.insert(entity); }
		virtual void RemoveEntity(IEntity* entity) { m_entities.erase(entity); }

		virtual void MarkForDeletion(IEntity* entity) { if (entity) m_entitiesToDelete.insert(entity); }
		static void Export(lua_State* L);

		CEngine* GetEngine(void) const { return m_engine; }

	private:
		typedef std::unordered_set<IEntity*> EntitySet;
		typedef std::unordered_set<IAnimation*> AnimationSet;

		EntitySet m_entities;
		EntitySet m_entitiesToDelete;
		AnimationSet m_animations;
		CEngine* m_engine;
	};

	class CGameManager: public IGameManager
	{
	public:
		CGameManager(CEngine* engine): IGameManager(engine) { }
	};
}