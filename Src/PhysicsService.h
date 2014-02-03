#pragma once

class b2World;
namespace VE
{
	class IPhysicsService
	{
	public:
		virtual void Simulate(void) const = 0;
		virtual void DrawDebugData(void) const = 0;
		virtual b2World* World(void) const = 0;
	};
}