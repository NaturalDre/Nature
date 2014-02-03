#include "Process.h"
#include "Engine.h"
#include <assert.h>

namespace VE
{
	IProcess::IProcess(CEngine* engine, size_t priority)
		: m_shouldDelete(false)
		, m_priority(priority)
		, m_engine(engine)
	{
		assert(m_priority > VENGINE_PROCESS_END);
		//if (m_engine)
		//	m_engine->AddProcess(this);
	}

	IProcess::~IProcess(void)
	{
		//if (m_engine)
		//	m_engine->RemoveProcess(this);
	}
}