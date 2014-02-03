#include "Player.h"
#include "PlayerController.h"
#include <iostream>
#include "GameManager.h"
#include "PlayerEvents.h"
#include "PlayerView.h"
#include "Locator.h"
#include "Engine.h"
#include "PhysicsComponent.h"


using namespace VE;

#define EVENT_SPEEDCHANGE ALLEGRO_GET_EVENT_TYPE('S', 'P', 'D', 'C')
#define EVENT_DIRCHANGE ALLEGRO_GET_EVENT_TYPE('D', 'I', 'R', 'C')

// Calculate impulse needed to simulate moving the desiredVel over one second.
float ImpulseForDistanceX(float desiredVel, b2Body* body)
{
	b2Vec2 vel = body->GetLinearVelocity();
	float velChange = desiredVel - vel.x;
	float impulse = body->GetMass() * velChange;
	return impulse;
}

b2Vec2 ImpulseForDistance(const b2Vec2& desiredVel, b2Body* body)
{
	b2Vec2 vel = body->GetLinearVelocity();
	float xVelChange = desiredVel.x - vel.x;
	float yVelChange = desiredVel.y - vel.y;

	b2Vec2 impulse(0,0);

	impulse.x = body->GetMass() * xVelChange;
	impulse.y = body->GetMass() * yVelChange;

	return impulse;
}

namespace VE
{
	CPlayer::CPlayer(IGameManager* level, const b2Vec2& spawnPos)
		: IEntity(level)
		, m_body(nullptr)
		, m_view(nullptr)
		, m_speed(0,0)
		, m_vel(5,5)
	{
		CPhysics* physics = CLocator::GetGameManager()->GetEngine()->GetPhysics();
		m_body = new CPhysicsComponent(physics, CreateBody(physics, spawnPos));
		
		
		
		// Set the initial direction of the player.
		SetDirection(RIGHT);
		CPlayerView* view = new CPlayerView(this);
		m_view = view;
		CPlayerController* c = new CPlayerController(this);
		Add(c);
	}

	CPlayer::~CPlayer(void)
	{
		delete m_body;
		m_body = nullptr;

		delete m_view;
		m_view = nullptr;
	}

	b2Body* CPlayer::CreateBody(CPhysics* physics, const b2Vec2& spawnPos)
	{
		//assert(physics != nullptr);
		if (physics == nullptr)
			return nullptr;

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.allowSleep = false;
		bd.bullet = true;
		bd.position = spawnPos;
		//bd.userData = static_cast<void*>(this);
		bd.fixedRotation = true;

		b2FixtureDef fd;
		fd.density = 0.3f;
		fd.friction = 0.3f;
		fd.restitution = 0.0f;
		//fd.userData = this;
		fd.filter.categoryBits = CATEGORY_PLAYER;
		fd.filter.maskBits = MASK_PLAYER;

		b2PolygonShape shape;
		fd.shape = &shape;

		shape.SetAsBox(0.5, 0.5);

		b2Body* body = physics->World()->CreateBody(&bd);
		body->CreateFixture(&fd);

		return body;
	}

	/////////////////////////////////////
	////////// CPLAYER //////////////////
	/////////////////////////////////////

	b2Vec2 CPlayer::GetPosition(void) const { return m_body->GetPosMtrs(); }

	void CPlayer::SetXSpeed(float x) 
	{ 
		if (GetXSpeed() != x)
		{
			m_speed.x = x; 
			Notify(EVENT_SPEEDCHANGE);
		}
	}

	void CPlayer::SetYSpeed(float y) 
	{ 
		if (GetYSpeed() != y)
		{
			m_speed.y = y; 
			Notify(EVENT_SPEEDCHANGE);
		}
	}

	void CPlayer::SetSpeed(const b2Vec2& speed) 
	{ 
		if (!(GetSpeed() == speed))
		{
			m_speed = speed; 
			Notify(EVENT_SPEEDCHANGE);
		}
	}

	void CPlayer::Update(double dt)
	{
		IEntity::Update(dt);
		b2Body* body = m_body->GetRaw();
		body->ApplyLinearImpulse(b2Vec2(ImpulseForDistance(m_speed, body)), body->GetWorldCenter()); 
	}

	void CPlayer::SetDirection(DIRECTION dir)
	{
		// Only update direction if it is a new direction
		if (m_dir != dir)
		{
			m_dir = dir;
			Notify(EVENT_DIRCHANGE);
		}
	}

	//void CPlayer::Export(lua_State* L)
	//{
	//	using namespace luabind;
	//	module(L)
	//		[
	//			class_<CPlayer, bases<IEntity>>("CPlayer")
	//			.property("speedX", &CPlayer::GetXSpeed, &CPlayer::SetXSpeed)
	//			.property("speedY", &CPlayer::GetYSpeed, &CPlayer::SetYSpeed)
	//			.property("speed", &CPlayer::GetSpeed, &CPlayer::SetSpeed)
	//			.property("position", &CPlayer::GetPosition)
	//		];

	//	module(L)
	//		[
	//			def("CreatePlayer", &VE::CreatePlayer, adopt(result))
	//		];
	//}

	CPlayer* VE::CreatePlayer(IGameManager* gameManager, const b2Vec2& spawnPos)
	{
		return new CPlayer(gameManager, spawnPos);
	}
}