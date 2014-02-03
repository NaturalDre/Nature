#ifndef IEVENT_H
#define IEVENT_H

#include <allegro5\allegro5.h>
#include <string>

namespace VE
{
	class IEvent
	{
	protected:
		IEvent(int type = 0): m_type(type) { }


	public:
		virtual ~IEvent(void) = 0 { }
		double Time(void) const { return m_time; }
		void SetTime(double time) { m_time = time; }

		std::string Description(void) { return m_description; }
		void SetDescription(const std::string& desc) { m_description = desc; }

		int Type(void) const { return m_type; }

	private:
		int m_type;
		double m_time;
		std::string m_description;

	};
}

#endif