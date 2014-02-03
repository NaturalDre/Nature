#ifndef ASSET_H
#define ASSET_H

#include <string>

class IAsset
{
public:
	enum { UNKNOWN = 0, GRAPHICAL, AUDIO, SCRIPT };

	IAsset(const std::string& filename, size_t type, int scene): m_filename(filename), m_type(type), m_scene(scene), m_loaded(false) { }
	virtual ~IAsset(void) { }
	/// The filename of the asset.
	const std::string& GetFilename(void) const { return m_filename; }
	/// The type of asset.
	unsigned int GetType(void) const { return m_type; }
	/// The scene this asset belongs to.
	unsigned int GetScene(void) const { return m_scene; }
	/// Is this asset loaded?
	bool IsLoaded(void) const { return m_loaded; }
	/// Loads the asset.
	/// @note Only call if you instantiated the object. As opposed to getting a pointer to it from CAssetManager.
	void Load(void) { OnLoad(); }
	/// Unloads the asset.
	/// @note Only call if you instantiated the object. As opposed to getting a pointer to it from CAssetManager.
	void Unload(void) { OnUnload(); }

protected:
	void SetLoaded(bool isLoaded) { m_loaded = isLoaded; }
	void SetFilename(const std::string& filename) { m_filename = filename; }

	virtual void OnLoad(void) = 0;
	virtual void OnUnload(void) = 0;

private:
	std::string m_filename; /// Filename of this asset.
	size_t m_type; /// Numeric id for the type of asset this is.
	int m_scene; /// The scene this asset is associated with.
	bool m_loaded; /// Is this asset currently loaded?
};

#endif