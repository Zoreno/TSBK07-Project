#include "AssetManager.h"
#include "iostream"

AssetManager::AssetManager() :
	_assets{}
{
	registerAsset<RawModel>();
	registerAsset<Texture2D>();
}

template <typename T>
void AssetManager::registerAsset()
{
	// Generate a new ID for the component
	size_t id = std::type_index{ typeid(T) }.hash_code();

	auto it = _assets.find(id);

	if (it == _assets.end())
		throw AssetManager_error{ "Asset is already registered." };

	_assets.emplace(id, new AssetPool<T>{});
}

template <typename T>
T* AssetManager::fetch(const std::string& ID)
{
	AssetPool<T>* pool = getPool<T>();

	return pool->fetchAsset(ID);
}

template <typename T>
void AssetManager::load(const std::string& path, std::string ID)
{
	AssetPool<T>* pool = getPool<T>();

	pool->loadAsset(path, ID);
}

template <typename T>
void AssetManager::dispose(const std::string& ID)
{
	AssetPool<T>* pool = getPool<T>();

	pool->removeAsset(ID);
}

template <typename T>
AssetPool<T>* AssetManager::getPool()
{
	size_t id = std::type_index{ typeid(T) }.hash_code();

	auto it = _assets.find(id);

	if (it == _assets.end())
		throw AssetManager_error{ "Asset is not registered yet." };

	AssetPool<T>* pool = dynamic_cast<AssetPool<T>*>(it->second);

	return pool;
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