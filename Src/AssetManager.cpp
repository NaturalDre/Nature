#include "AssetManager.h"
#include "Bitmap.hpp"
#include <iostream>
#include "tinyxml.h"
#include "BitmapAsset.h"

CAssetManager* CAssetManager::instance = nullptr;

CAssetManager::CAssetManager(void)
	: m_curScene(0)
{
	
}

void CAssetManager::LoadAssetsFromLua(const std::string& filename)
{
	std::cout << "\nAttempting to load assets from lua file '" << filename << "'.\n";
	std::cout << "\nFinished loading assets from lua file '" << filename << "'.\n";
}

void CAssetManager::LoadAssetsFromXML(const std::string& filename)
{
	std::cout << "Attempting to load assets from XML file '" << filename << "'.\n";
	TiXmlDocument doc(filename.c_str());
	if (doc.LoadFile())
	{
		std::cout << "File loaded.\n";
		// Get the root node.
		TiXmlNode* assetTree = doc.FirstChild("Assets");

		// If we found no root node, return.
		if (assetTree)
		{
			std::cout << "Root node found.\n";
			// Iterate over all children in the root
			for (TiXmlNode* child = assetTree->FirstChild(); child; child = child->NextSibling())
			{
				// Convert current node to an element.
				TiXmlElement* element = child->ToElement();
				// Obtain info if conversion succeeded.
				if (element)
				{
					std::cout << "Found an element.\n";
					std::shared_ptr<IAsset> asset = nullptr;

					const std::string filename = element->Attribute("filename");
					const std::string type = element->Attribute("type");
					const int scene = atoi(element->Attribute("scene"));

					if (type == "GRAPHICAL")
					{
						asset = std::shared_ptr<IAsset>(new VE::CBitmapAsset(filename));
					}
					else if (type == "AUDIO")
					{

					}
					else if (type == "SCRIPT")
					{

					}

					if (asset)
					{
						asset->Load();
						m_assets[scene].push_back(std::shared_ptr<IAsset>(asset));
						std::cout << "Asset loaded. \n";
					}
					else
						std::cout << "Asset did not load.\n";
				}
			}
		}
	}
	std::cout << "Finished loading assets from XML file '" << filename << "'.\n";
}

std::shared_ptr<IAsset> CAssetManager::GetAsset(const std::string& name)
{
	auto& v = m_assets[m_curScene];
	for (auto iter = v.begin(); iter != v.end(); ++iter)
	{
		if ((*iter)->GetFilename() == name)
			return *iter;
	}
	std::cout << "Unable to find asset: '" << name << "'.\n";
	return nullptr;
}