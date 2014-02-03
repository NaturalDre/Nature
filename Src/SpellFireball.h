#pragma once
#include "Entity.h"
#include "PhysicsComponent.h"
#include <Box2D\Box2D.h>

namespace VE
{
	class CEngine;
}

namespace Game1
{
	class CSpellFireball: public VE::IEntity
	{
	public:
		CSpellFireball(VE::CEngine* engine, const b2Vec2& spawnPos);
		~CSpellFireball(void);

	protected:
		void CreateBody(const b2Vec2& spawnPos);
		// TODO: Add Update function that manages lifespan.
	private:
		VE::CEngine* m_engine;
		VE::CPhysicsComponent* m_body;

	};
};