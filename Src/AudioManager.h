#pragma once

#include <unordered_set>

namespace VE
{
	class CEngine;
	class IAudioComponent;
	class CAudioManager
	{
	public:
		CAudioManager(CEngine* engine);
		~CAudioManager(void);

		void Add(IAudioComponent* c);
		void Remove(IAudioComponent* c);

	private:
		CEngine* m_engine;
		std::unordered_set<IAudioComponent*> m_components;
	};
}