#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <set>
#include <Box2D\Box2D.h>
#include <string>
class b2Contact;

namespace VE
{
	typedef std::set<const std::string> EntityGroups;

	class IGameManager;
	class IComponent;
	class IEntity
	{
	public:
		IEntity(IGameManager* gameManager);

		virtual ~IEntity(void) = 0;
		virtual void Update(double dt);

		inline IGameManager* GetGameManager(void) const { return m_gameManager; }

		virtual bool OnContact(IEntity* entity) { return false; } // Return true to accept he contact


		void AddEntityGroup(const std::string& group) { m_entityGroups.insert(group); }
		void RemoveEntityGroup(const std::string& group) { m_entityGroups.erase(group); }

		void Add(IComponent* c) { m_components.push_back(c); }
		void Remove(IComponent* c) { m_components.remove(c); }

		virtual b2Vec2 GetPosition(void) const { return b2Vec2(0, 0); }

		const std::string& GetGroupName(void) const { return m_groupName; }
		inline size_t GetEntityID(void) const { return m_entityID; }

		bool IsEnabled(void) const { return m_enabled; }
		void SetEnabled(bool enabled) { m_enabled = enabled; }

	private:
		static size_t m_count;

		IGameManager* m_gameManager; // Game level this entity is a part of.
		std::list<IComponent*> m_components;
		std::string m_groupName;
		const size_t m_entityID;
		bool m_enabled;
		EntityGroups m_entityGroups;
	};
}
#endif