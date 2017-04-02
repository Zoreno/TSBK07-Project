#pragma once

#include <string>
#include <map>
#include <utility>
#include <stdexcept>

#include "Texture2D.h"
#include "loadobj.h"

//Error class
class AssetManager_error : public std::logic_error {
	using std::logic_error::logic_error;
};

class AssetManager
{
public:
	//Only default constructor needed
	AssetManager() = default;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
	~AssetManager() = default;

protected:
	//loaders
	static void loadModel(std::string filename, std::string ID);
	static void loadTexture(std::string filename, std::string ID);

	//getters

	//removers
	static void disposeModel(std::string ID);
	static void disposeTexture(std::string ID);

	//containers
	static std::map<std::string, Model*> models;
	static std::map<std::string, Texture2D*> textures;
	
};

