#include "ContactListener.h"
#include "PhysicsComponent.h"

namespace VE
{
	void CContactListener::BeginContact(b2Contact* contact)
	{
		b2Fixture* fA = contact->GetFixtureA();
		b2Fixture* fB = contact->GetFixtureB();

		if (fA)
		{
			CPhysicsComponent* cbA = static_cast<CPhysicsComponent*>(fA->GetUserData());
			if (cbA)
				cbA->BeginContact(contact);
		}
		if (fB)
		{
			CPhysicsComponent* cbB = static_cast<CPhysicsComponent*>(fB->GetUserData());
			if (cbB)
				cbB->BeginContact(contact);
		}
	}

	void CContactListener::EndContact(b2Contact* contact)
	{
		b2Fixture* fA = contact->GetFixtureA();
		b2Fixture* fB = contact->GetFixtureB();

		if (fA)
		{
			CPhysicsComponent* cbA = static_cast<CPhysicsComponent*>(fA->GetUserData());
			if (cbA)
				cbA->EndContact(contact);
		}
		if (fB)
		{
			CPhysicsComponent* cbB = static_cast<CPhysicsComponent*>(fB->GetUserData());
			if (cbB)
				cbB->EndContact(contact);
		}
	}

	void CContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		/* 
			Temporarily disabled the code below, because I do not see
			a use for it yet.
		*/

		//b2Fixture* fA = contact->GetFixtureA();
		//b2Fixture* fB = contact->GetFixtureB();

		//if (fA)
		//{
		//	CPhysicsComponent* cbA = static_cast<CPhysicsComponent*>(fA->GetUserData());
		//	if (cbA)
		//		cbA->PreSolve(contact, oldManifold);
		//}
		//if (fB)
		//{
		//	CPhysicsComponent* cbB = static_cast<CPhysicsComponent*>(fB->GetUserData());
		//	if (cbB)
		//		cbB->PreSolve(contact, oldManifold);
		//}
	}

	void CContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		/* 
			Temporarily disabled the code below, because I do not see
			a use for it yet.
		*/

		//b2Fixture* fA = contact->GetFixtureA();
		//b2Fixture* fB = contact->GetFixtureB();

		//if (fA)
		//{
		//	CPhysicsComponent* cbA = static_cast<CPhysicsComponent*>(fA->GetUserData());
		//	if (cbA)
		//		cbA->PostSolve(contact, impulse);
		//}
		//if (fB)
		//{
		//	CPhysicsComponent* cbB = static_cast<CPhysicsComponent*>(fB->GetUserData());
		//	if (cbB)
		//		cbB->PostSolve(contact, impulse);
		//}
	}
}