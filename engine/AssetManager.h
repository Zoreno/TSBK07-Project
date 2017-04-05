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

	//loaders
	void loadModel(std::string filename, std::string ID);
	void loadTexture(std::string filename, std::string ID);

	//getters
	Model* fetchModel(std::string ID);
	Texture2D* fetchTexture(std::string ID);
protected:

	//removers
	void disposeModel(std::string ID);
	void disposeTexture(std::string ID);

	//containers
	std::map<std::string, Model*> _models{};
	std::map<std::string, Texture2D*> _textures{};
	
};

