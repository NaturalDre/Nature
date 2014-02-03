#ifndef PROCESS_H
#define PROCESS_H

namespace VE
{
	class CEngine;
	class IProcess
	{
	public:
		enum
		{
			VENGINE_PROCESS_END = 100
		};

		IProcess(CEngine* engine = nullptr, size_t priority = VENGINE_PROCESS_END + 1);
		virtual ~IProcess(void);

		inline CEngine* GetEngine(void) const { return m_engine; }
		inline size_t GetPriority(void) const { return m_priority; }


		virtual void Think(double dt) = 0;
		/// \return True if CEngine should free this process at the beginning
		/// of the next frame. False if not.
		inline bool ShouldDelete(void) { return m_shouldDelete; }
		/// @param shouldDelete Do you want CEngine to free this process at the start
		/// of the next frame?
		inline void SetShouldDelete(bool shouldDelete) { m_shouldDelete = shouldDelete; }

		inline bool operator>(const IProcess& process) { return GetPriority() > process.GetPriority(); }
		inline bool operator<(const IProcess& process) { return GetPriority() < process.GetPriority(); }

	private:
		bool m_shouldDelete;
		size_t m_priority;
		CEngine* m_engine;
	};
}
#endif