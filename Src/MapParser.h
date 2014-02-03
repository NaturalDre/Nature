#ifndef MAPPARSER_H
#define MAPPARSER_H

namespace VE
{
	class CGameMap;
	class IGameManager;
	class CMapParser               
	{
	protected:
		void DoMapProperties(void);
		void DoTileLayers(void);

		void RemoveRuleGids(void);
	public:
		CMapParser(void) { }
		CMapParser(IGameManager* gameManager, CGameMap* gameMap);
		~CMapParser(void);

		void Parse(void);

	private:
		IGameManager* m_gameManager;
		CGameMap* m_gameMap;
	};
}
#endif