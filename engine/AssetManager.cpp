/**
 * @file	AssetManager.cpp
 * @Author	Kevin Kjellén
 * @date	2017-05-21
 * @brief	AssetManager
 */

#include "AssetManager.h"
#include "iostream"

AssetManager::AssetManager() :
	_assets{}
{
	registerAsset<RawModel>();
	registerAsset<Texture2D>();
}

AssetManager::~AssetManager()
{
	for (auto& i : _assets)
	{
		delete i.second;
	}
}


/*
void AssetManager::loadModel(std::string filename, std::string ID)
{
	auto insPair = _models.insert(std::pair<std::string, Model*>(ID, nullptr));
	if (insPair.second){
		char *cstr = new char[filename.length() + 1];
		strcpy(cstr, filename.c_str());

		Model* m{ nullptr };
		try {
			m = LoadModelPlus(cstr);
		}
		catch (...) {
			delete[] cstr;
			_models.erase(insPair.first);
			DisposeModel(m);
			throw AssetManager_error(filename.append(": failed to load model file"));
		}
		insPair.first->second = m;
		delete[] cstr;
	}
	else {
		throw AssetManager_error(ID.append(": duplicate model ID"));
	}

	return;
}

void AssetManager::loadTexture(std::string filename, std::string ID)
{
	auto insPair = _textures.insert(std::pair<std::string, Texture2D*>(ID, nullptr));

	if (insPair.second) {
		try {
			insPair.first->second = new Texture2D{filename.c_str()};
		}
		catch (...) {
			throw AssetManager_error(filename.append(": failed to load texture file"));
		}
	}
	else {
		throw AssetManager_error(ID.append(": duplicate texture ID"));
	}
	return;
}

Model* AssetManager::fetchModel(std::string ID)
{
	auto modIter = _models.find(ID);

	if (modIter == _models.end())
		throw AssetManager_error(ID.append(": model ID not found"));
	else
		return modIter->second;
}

Texture2D* AssetManager::fetchTexture(std::string ID)
{
	auto texIter = _textures.find(ID);

	if (texIter == _textures.end())
		throw AssetManager_error(ID.append(": model ID not found"));

	std::cout << "fetching!!" << std::endl;
	return texIter->second;
}



void AssetManager::disposeModel(std::string ID)
{
	auto iterator = _models.find(ID);

	if (iterator == _models.end()) {
		throw(ID.append(": model ID not found and could not be disposed"));
	}
	else {
		DisposeModel(iterator->second);
		_models.erase(iterator);
	}
	return;
}

void AssetManager::disposeTexture(std::string ID)
{
	auto iterator = _textures.find(ID);

	if (iterator == _textures.end()) {
		throw(ID.append(": model ID not found and could not be disposed"));
	}
	else {
		delete iterator->second;
		_textures.erase(iterator);
	}
	return;
}
*/