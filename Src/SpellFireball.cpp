#include "SpellFireball.h"
#include "Engine.h"
#include "GameManager.h"
#include "Physics.h"
#include <assert.h>

namespace Game1
{
	CSpellFireball::CSpellFireball(VE::CEngine* engine, const b2Vec2& spawnPos)
		: VE::IEntity(engine->GetGameManager())
		, m_body(nullptr)
	{

	}

	CSpellFireball::~CSpellFireball(void)
	{

	}

	void CSpellFireball::CreateBody(const b2Vec2& spawnPos)
	{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.allowSleep = false;
		bd.bullet = true;
		bd.position = spawnPos;
		bd.fixedRotation = true;

		b2FixtureDef fd;
		fd.density = 0.3f;
		fd.friction = 0.3f;
		fd.restitution = 0.0f;
		
		b2PolygonShape shape;
		fd.shape = &shape;
		shape.SetAsBox(2,2);

		{
			b2Body* body = m_engine->GetPhysics()->World()->CreateBody(&bd);
			m_body = new VE::CPhysicsComponent(m_engine->GetPhysics(), body);
		}
		m_body->GetRaw()->SetUserData(static_cast<void*>(m_body));

		Add(m_body);
	}
}