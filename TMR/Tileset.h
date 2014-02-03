#ifndef TILETILESET_H
#define TILETILESET_H

#include <string>
#include <list>
#include <vector>
#include <map>
#include "Layer.h"
//#include "..\Bitmap.h"

struct lua_State;
struct ALLEGRO_BITMAP;

namespace luabind
{
	namespace adl
	{
		class object;
	}
}
struct lua_State;

namespace Tiled
{
	class Tile
	{
	protected:

	public:
		typedef std::map<const std::string, const std::string> Properties;

		Tile(void): m_id(0) { }
		Tile(size_t id, const Properties& properties): m_id(id), m_properties(properties) { }
		Tile(size_t id, Properties&& properties): m_id(id), m_properties(std::move(properties)) { }

		size_t GetID(void) const { return m_id; }
		const Properties& GetProperties(void) { return m_properties; }

		void SetID(size_t id) { m_id = id; }
		void SetProperties(const Properties& props) { m_properties = props; }
		void SetProperties(Properties&& props) { m_properties = std::move(props); }
	private:

		size_t m_id;
		Properties m_properties;
	};

	class CTileset
	{
	protected:
		static std::list<Tile> ReadTiles(lua_State* L, size_t index);
		static void LoadTilesetProperties(CTileset& ts, const luabind::adl::object& data);
		static std::list<Tile> LoadTilesetTiles(CTileset& ts, const luabind::adl::object& data);
		static void LoadTilesetProperties(CTileset& ts, lua_State* L);
		void LoadDefaults(void);

	public:
		CTileset(const luabind::adl::object& tileset);
		CTileset(lua_State* L);
		~CTileset(void);

		inline std::string Name(void) const { return m_name; }
		inline std::string Source(void) const { return m_source; }
		inline std::string Trans(void) const { return m_trans; }

		inline size_t FirstGid(void) const { return m_firstGid; }
		inline size_t LastGid(void) const { return m_lastGid; }

		inline size_t TileWidth(void) const { return m_tileWidth; }
		inline size_t TileHeight(void) const { return m_tileHeight; }
		inline size_t ImageWidth(void) const { return m_tileWidth; }
		inline size_t ImageHeight(void) const { return m_tileHeight; }
		inline size_t Spacing(void) const { return m_spacing; }
		inline size_t TilesAcross(void) const { return m_tilesAcross; }
		inline size_t TilesDown(void) const { return m_tilesDown; }

		inline bool IsValid(void) const { return m_isValid; }
		/*
		* ContainsGid()
		*
		* Check if this tileset contains the gid
		*/
		bool ContainsGid(size_t gid) const { return ((gid >= m_firstGid) && (gid <= m_lastGid)); }
		/*
		* ContainsGid()
		*
		* Check if a list of tilsets contain the gid
		*/
		static CTileset* ContainsGid(const std::vector<CTileset*>& tilesets, size_t id);
		/*
		* LoadTile()
		*
		* Checks a list of tilesets to find which one contains the gid and then loads and returns it.
		*
		* Notes: 
		*	You must free the memory. 
		* CAUTION: 
		*	If you don't free the memory before the tileset that you got it 
		*	from is destroyed the behavior will be undefined.
		*/
		//static VE::CBitmap LoadTile(const std::vector<CTileset*>& tilesets, size_t id);
		/*
		* GetImage()
		*
		* Get the image file.
		*
		* Notes:
		*	Null pointer if it wasn't able to find and load the image. 
		*	The image isn't loaded by this function, it is loaded when ReadMapFile() is called.
		*/
		//const VE::CBitmap&  GetImage(void) const { return m_image; }

	private:
		bool m_isValid;

		std::string m_name;
		std::string m_source;
		std::string m_trans;

		size_t m_firstGid;
		size_t m_lastGid;

		size_t m_tileWidth;
		size_t m_tileHeight;
		size_t m_imageWidth;
		size_t m_imageHeight;
		size_t m_spacing;
		size_t m_tilesAcross;
		size_t m_tilesDown;

		//VE::CBitmap m_image;

		std::list<Tile> m_tiles;
	};

	namespace Key
	{
		namespace TileSet
		{
			const std::string NAME = "name";
			const std::string FIRSTGID = "firstgid";
			const std::string TILEWIDTH = "tilewidth";
			const std::string TILEHEIGHT = "tileheight";
			const std::string SPACING = "spacing";
			const std::string IMAGE = "image";
			const std::string IMAGEWIDTH = "imagewidth";
			const std::string IMAGEHEIGHT = "imageheight";
			const std::string TRANSPARENTCOLOR = "transparentcolor";
		}
	}
}

#endif