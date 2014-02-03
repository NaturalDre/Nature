#include "MapParser.h"
#include "GameMap.h"
#include <boost\lexical_cast.hpp>
#include "Utility.h"

namespace VE
{

	CMapParser::CMapParser(IGameManager* gameManager, CGameMap* gameMap)
		: m_gameManager(gameManager)
		, m_gameMap(gameMap)
	{

	}


	CMapParser::~CMapParser(void)
	{
		m_gameManager = nullptr;
	}

	void CMapParser::Parse(void)
	{
		if (!m_gameManager || !m_gameMap || !m_gameMap->IsValid())
			return;

		DoMapProperties();
		//RemoveRuleGids();
	}

	void CMapParser::DoMapProperties(void)
	{
		{
			const int index = m_gameMap->FindTileLayerIndex(m_gameMap->GetProperty("PlayerLayer"));
			if (index >= 0)
				m_gameMap->SetPlayerLayer(index);
		}
		{
			try
			{
				float x = boost::lexical_cast<float>(m_gameMap->GetProperty("SpawnX"));
				float y = boost::lexical_cast<float>(m_gameMap->GetProperty("SpawnY"));
				m_gameMap->SetPlayerSpawn(x * m_gameMap->GetTileWidth(), y * m_gameMap->GetTileHeight());
			}
			catch(const std::exception&)
			{
				// TODO: Handle exception.
				//VASSERT(false, "SpawnX and SpawnY are not variables in the map properties.");
			}

		}
	}

	void CMapParser::RemoveRuleGids(void)
	{
		Tiled::CTileset* ts = m_gameMap->GetTileset("rules");
		if (!ts)
			return;		

		for (auto layerIter = m_gameMap->GetTileLayers().begin(); layerIter != m_gameMap->GetTileLayers().end(); ++layerIter)
		{
			for (size_t i = ts->FirstGid(); i <= ts->LastGid(); ++i)
				(*layerIter)->ReplaceAll(i, 0);
		}
	}
}