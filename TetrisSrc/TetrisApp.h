#pragma once


namespace VE {
	class CEngine;
}

namespace Tetris
{
	class CTetrisApp
	{
	public:
		CTetrisApp(void);
		~CTetrisApp(void);

		int Run(void);

	protected:
		void Init(void);
		void SetupDirectories(void);
		void PreLogic(void);

	private:
		VE::CEngine* m_engine;
	};
}