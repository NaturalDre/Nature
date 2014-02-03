#ifndef PLAYER_H
#define PLAYER_H

#include "Physics.h"
#include "Entity.h"
#include "Observable.h"
#include "Common.h"
#include "PhysicsComponent.h"

namespace VE
{
	class CPlayerBody;
	class IView;
	class CPlayer: public IEntity, public IObservable
	{
		friend CPlayer* CreatePlayer(IGameManager* gameManager, const b2Vec2& spawnPos);
	protected:
		CPlayer(IGameManager* gameManager, const b2Vec2& spawnPos);

	public:

		virtual ~CPlayer(void);

		/**
		* Called every frame for the object to update itself based on the current state of the game.
		* @param deltatime Amount of time since Update was last called.
		*/
		void Update(double deltatime);
		/**
		* Returns the position of the player.
		* @return The position in meters.
		*/
		b2Vec2 GetPosition(void) const;
		void SetXSpeed(float x);
		void SetYSpeed(float y);
		void SetSpeed(const b2Vec2& speed);

		float GetXSpeed(void) const { return m_speed.x; }
		float GetYSpeed(void) const { return m_speed.y; }
		b2Vec2 GetSpeed(void) const { return m_speed; }


		DIRECTION GetDirection(void) const{ return m_dir; }
		void SetDirection(DIRECTION dir);

		static b2Body* CreateBody(CPhysics* physics, const b2Vec2& spawnPos);

		///// Subscribe to a specific set of events that this object generates.
		/////
		///// List of Topics: Currently there are none.
		///// @param topic The kinds of events you want to listen for. 
		///// @param observer The object who will be notified of these events.
		//void SubscribeTo(const std::string& topic, IObserver* observer);
		///// Subscribe from a set of events that this object generates.
		/////
		///// @param topic The topic you no longer to to be notified about.
		///// @param observer The object that no longer wants these events.
		//void SubscribeFrom(const std::string& topic, IObserver* observer);
		/////
		///// Subscribe from all events that this object generates.
		/////
		///// @param observer The object that no longer wants to listen to events from this object.
		//void SubscribeFromAll(IObserver* observer);

		static void Export(lua_State* L);
	private:
		//CPlayerBody* m_body;
		CPhysicsComponent* m_body;
		IView* m_view;
		b2Vec2 m_speed;
		b2Vec2 m_vel;
		DIRECTION m_dir;
	};

	CPlayer* CreatePlayer(IGameManager* gameManager, const b2Vec2& spawnPos);
}
#endif