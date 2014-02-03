#pragma once

#include "TMR\MapFile.h"
#include <Bitmap.hpp>
#include "MapParser.h"
#include <map>
#include "Physics.h"

struct lua_State;

namespace VE
{
	class CRender;
	class IGameManager;
	class IView;
	class CGameMap: public Tiled::CMapFile
	{
		friend CMapParser;
	public:
		typedef std::vector<Tiled::CTileLayer*>::const_iterator ConstTileLayerIter;

	protected:
		ConstTileLayerIter FindTileLayer(const std::string& name);
		/// Finds the tilelayer with the name @name and returns its index.
		/// \note Returns -1 on failure to find layer with @name.
		int FindTileLayerIndex(const std::string& name);


		void SetPlayerSpawn(float x, float y) { m_playerSpawn = PixToMtr(b2Vec2(x,y)); }
		/// @param pos Position on the map to spawn the player in pixels.
		void SetPlayerSpawn(const b2Vec2& pos) { m_playerSpawn = pos; };
	public:

		CGameMap(IGameManager* gameManager);
		~CGameMap(void) { }

		bool Read(const std::string& filename);

		inline size_t GetPlayerLayer(void) const { return m_playerLayer; }
		/// @param name The name of the tileset you want.
		/// \return A pointer to the tileset. nullptr if none found.
		const alx::Bitmap GetTilesetImage(const std::string& name) const;
		/// \return Position in meters.
		b2Vec2 GetPlayerSpawn(void) const { return m_playerSpawn; }

		inline void SetPlayerLayer(const std::string& layerName) { m_playerLayer = FindTileLayerIndex(layerName); }
		inline void SetPlayerLayer(size_t layerIndex) { m_playerLayer = layerIndex; }

		static void Export(lua_State* L);
	private:
		std::map<std::string, alx::Bitmap> m_tilesetImages;
		CMapParser m_parser;
		IGameManager* m_gameManager;
		size_t m_playerLayer;
		b2Vec2 m_playerSpawn;
		Tiled::Object m_playerData;
		/// For drawing layers on/below the layer the player is on.
		IView* m_lowerView;
		/// For drawing layers above the layer the player is on.
		IView* m_upperView;
	};
}