#ifndef TILEDLAYER_H
#define TILEDLAYER_H

#include "LayerTypes.h"
#include <string>
#include <map>

struct lua_State;

namespace Tiled
{
	class ILayer
	{
	protected:
		typedef std::map<const std::string, const std::string> Properties;
		/// \note Table MUST be at top of stack, it MUST have a 'properties' key, 
		/// and all values MUST be strings 
		static void ReadProperties(lua_State* L, ILayer* layer);
	public:
		ILayer(void):m_width(0), m_height(0), m_opacity(1.0f), m_visible(true) { }

		virtual int GetType(void) const =  0 { return e_Undefined; }

		inline std::string GetName(void) const { return m_name; }
		inline size_t GetWidth(void) const { return m_width; }
		inline size_t GetHeight(void) const { return m_height; }
		inline float GetOpacity(void) const { return m_opacity; }
		inline const Properties& GetProperties(void) const { return m_properties; }
		std::string GetProperty(const std::string& prop);
		inline bool IsVisible(void) const { return m_visible; }

		inline void SetName(const std::string& name) { m_name = name; }
		inline void SetWidth(size_t width) { m_width = width; }
		inline void SetHeight(size_t height) { m_height = height; }
		inline void SetOpacity(float opacity) { m_opacity = opacity; }
		inline void SetVisible(bool visible) { m_visible = visible; }

	private:
		std::string m_name;
		Properties m_properties;
		size_t m_width;
		size_t m_height;
		float m_opacity;
		bool m_visible;
	};
}
#endif