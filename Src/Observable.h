#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <unordered_map>
#include <unordered_set>
#include <string>

namespace VE
{
	class IObserver;
	typedef std::unordered_map<int,  std::unordered_set<IObserver*>> ObserverTopics;

	class IObservable
	{
	protected:

	public:
		IObservable(void) { }
		virtual ~IObservable(void) = 0 { }

		void Notify(int eventType);
		void Notify(int eventType, int topic);
		// Register with specific topic
		void Register(int topic, IObserver* observer);
		// Deregister from specific topic
		void Deregister(int topic, IObserver* observer);
		// Deregister from all topics.
		void Deregister(IObserver* observer);

	private:
		ObserverTopics m_topics;

		IObservable(const IObservable&);
		void operator==(const IObservable&);
	};
}
#endif