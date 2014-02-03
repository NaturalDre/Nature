#ifndef OBSERVER_H
#define OBSERVER_H

namespace VE
{
	class IEvent;
	class IObserver
	{
	protected:

	public:
		virtual void Notify(int eventCode) { }
		virtual ~IObserver(void) = 0 {}
	private:

	};
}

#endif