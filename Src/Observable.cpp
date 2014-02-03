#include "Observable.h"
#include "Observer.h"
#include "Event.h"

namespace VE
{
	void IObservable::Notify(int eventType)
	{
		for(auto topicIter = m_topics.begin(); topicIter != m_topics.end(); ++topicIter)
			for(auto obsIter = topicIter->second.begin(); obsIter != topicIter->second.end(); ++obsIter)
				(*obsIter)->Notify(eventType);
	}

	void IObservable::Notify(int eventType, int topic)
	{
		auto iter = m_topics.find(topic);
		if (iter != m_topics.end())
			for (auto obsIter = iter->second.begin(); obsIter != iter->second.end(); ++obsIter)
				(*obsIter)->Notify(eventType);
	}

	void IObservable::Register(int topic, IObserver* observer)
	{
		m_topics[topic].insert(observer);
	}

	void IObservable::Deregister(int topic, IObserver* observer)
	{
		auto iter = m_topics.find(topic);
		if (iter != m_topics.end())
			iter->second.insert(observer);
	}

	void IObservable::Deregister(IObserver* observer)
	{
		for(auto topicIter = m_topics.begin(); topicIter != m_topics.end(); ++topicIter)
			topicIter->second.erase(observer);
	}
}