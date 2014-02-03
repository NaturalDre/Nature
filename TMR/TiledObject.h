#ifndef TILEDOBJECT_H
#define TILEDOBJECT_H

#include <map>
#include <string>

struct lua_State;

namespace luabind
{
	namespace adl
	{
		class object;
	}
}

namespace Tiled
{
	/*
	* struct TiledObject
	*
	* This represents the 'object' table that is found in Tiled's exort of
	* an 'objectlayer'.
	*
	*/
	class Object
	{
		typedef std::map<const std::string, const std::string> TiledObjectProperties;
	public:
		Object(void): m_x(0), m_y(0), m_width(0), m_height(0), m_isValid(false) {}
		Object(const luabind::adl::object& data);
		Object(lua_State* L);
		
		Object(Object&& rhs);

	public:
		inline const std::string& Name(void) const { return m_name; }
		inline const std::string& Type(void) const { return m_type; }
		inline float X(void) const { return m_x; }
		inline float Y(void) const { return m_y; }
		inline float Width(void) const { return m_width; }
		inline float Height(void) const { return m_height; }

		bool IsValid(void) const { return m_isValid; }

		static Object CreateFromLua(lua_State* L);
	private:
		std::string m_name;
		std::string m_type;
		std::string m_shape;
		float m_x;
		float m_y;
		float m_width;
		float m_height;
		bool m_visible;
		TiledObjectProperties m_properties;

		bool m_isValid; 
	};

	namespace Key
	{
		namespace TiledObject
		{
			static const std::string NAME = "name";
			static const std::string TYPE = "type";
			static const std::string SHAPE = "shape";
			static const std::string X = "x";
			static const std::string Y = "y";
			static const std::string WIDTH = "width";
			static const std::string HEIGHT = "height";
			static const std::string VISIBLE = "visible";
			static const std::string PROPERTIES = "properties";
		}
	};
}
#endif