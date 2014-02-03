#ifndef MAPFILE_H
#define MAPFILE_H

#include <map>
#include <vector>
#include <string>
#include <lua.hpp>
#include "Tilelayer.h"
#include "ObjectLayer.h"
#include "Tileset.h"

struct lua_State;

namespace VE
{
	class CRender;
}

namespace Tiled
{
	class CTileLayer;
	class CObjectLayer;

	class CMapFile
	{
	public:
		typedef std::vector<CTileLayer*> TileLayersVector;
		typedef std::vector<CObjectLayer*> ObjectLayersVector;
		typedef std::vector<CTileset*> TilesetsVector;

	protected:
		typedef std::map<const std::string, const std::string> MapProperties;
		static void LoadMapData(CMapFile& map, lua_State* L);
		static void LoadLayers(CMapFile& map, lua_State* L);

	public:
		CMapFile(void);
		virtual ~CMapFile(void);

		virtual bool Read(const std::string& mapFile);
		/// Version of the map exporter used to export this map.
		inline std::string GetVersion(void) const { return m_version; }
		inline std::string GetOrientation(void) const { return m_orientation; }
		/// Get the value of one of the map's properties.
		/// @param key The name ofthe property whose value you want.
		/// \return The value of the property. Empty string if not found.
		/// \note Some keys may have empty values.
		const std::string GetProperty(const std::string& key);
		/// \return An std::map of all the properties.
		inline const MapProperties GetProperties(void) const { return m_properties; }
		/// \return How many tiles wide the map is.
		inline size_t GetWidth(void) const { return m_width; }
		/// \return How many tiles high the map is.
		inline size_t GetHeight(void) const { return m_height; }
		/// \return How wide, in pixels, a tile is.
		inline size_t GetTileWidth(void) const { return m_tileWidth; }
		/// \return How tall, in pixels, a tile is.
		inline size_t GetTileHeight(void) const { return m_tileHeight; }
		/// \return A vector of all tilelayers.
		inline const TileLayersVector& GetTileLayers(void) const { return m_tileLayers; }
		/// \return A vector of all objectlayers.
		inline const ObjectLayersVector& GetObjectLayers(void) const { return m_objectLayers; }
		/// \return A vector of all tilesets.
		inline const TilesetsVector& GetTilesets(void) const { return m_tilesets; }
		/// @param name The name of the tileset you want.
		/// \return Valid pointer of the tileset if it was found, nullptr otherwise.
		CTileset* GetTileset(const std::string& name) const;
		/// Search all ObjectLayers for the first object of a specific name.
		/// @param The name of the object you want to find.
		/// \return Valid pointer if found, nullptr otherwise.
		/// \note Do not free the returned pointer and do not dereference it once 
		///		this map file has been reset or destructed.
		const Object* FindObject(const std::string& name) const;
		/// Search all ObjectLayers for all objects of a specific name.
		/// @param The name of object that you want to find all instances of.
		/// \return A list of all objects with the specified name.
		/// \note You are not responsible for freeing the pointer. Do not dereference
		///		once this object has been reset or destructed.
		std::list<const Object*> FindObjects(const std::string& name);

		/// \return True if this mapfile is valid. False otherwise.
		inline bool IsValid(void) const { return m_valid; }
		/// Discard all data that is loaded.
		void Reset(void);

	private:
		std::string m_version;
		std::string m_orientation;
		MapProperties m_properties;
		size_t m_width;
		size_t m_height;
		size_t m_tileWidth;
		size_t m_tileHeight;

		TileLayersVector m_tileLayers;
		ObjectLayersVector m_objectLayers;
		TilesetsVector m_tilesets;
		bool m_valid;
	};
}
#endif