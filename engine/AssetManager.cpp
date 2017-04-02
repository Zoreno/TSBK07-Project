#include "AssetManager.h"



std::map<std::string, Model*> AssetManager::models;
std::map<std::string, Texture2D*> AssetManager::textures;

void AssetManager::loadModel(std::string filename, std::string ID)
{
	auto insPair = models.insert(std::pair<std::string, Model*>(ID, nullptr));
	if (insPair.second){
		char *cstr = new char[filename.length() + 1];
		strcpy(cstr, filename.c_str());

		Model* m{ nullptr };
		try {
			m = LoadModelPlus(cstr);
		}
		catch (...) {
			delete[] cstr;
			models.erase(insPair.first);
			DisposeModel(m);
			throw AssetManager_error(filename.append(": failed to load model file"));
		}
		insPair.first->second = m;
		delete[] cstr;
	}
	else {
		throw AssetManager_error(ID.append(": duplicate model ID"));
	}

	return ;
}

void AssetManager::loadTexture(std::string filename, std::string ID)
{
	auto insPair = textures.insert(std::pair<std::string, Texture2D*>(ID, nullptr));

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

void AssetManager::disposeModel(std::string ID)
{
	auto iterator = models.find(ID);

	if (iterator == models.end()) {
		throw(ID.append(": model ID not found and could not be disposed"));
	}
	else {
		DisposeModel(iterator->second);
		models.erase(iterator);
	}
	return;
}

void AssetManager::disposeTexture(std::string ID)
{
	auto iterator = textures.find(ID);

	if (iterator == textures.end()) {
		throw(ID.append(": model ID not found and could not be disposed"));
	}
	else {
		delete iterator->second;
		textures.erase(iterator);
	}
	return;
}
