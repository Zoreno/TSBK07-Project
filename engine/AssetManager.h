#pragma once

#include <string>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <typeindex>
#include <memory>
#include <bitset>
#include <functional>
#include <iterator>
#include <stdexcept>

#include "Texture2D.h"
#include "loadobj.h"
#include "RawModel.h"

//Error class
class AssetManager_error : public std::logic_error {
	using std::logic_error::logic_error;
};

class AssetManager;

/**
* @brief Base class for the component pools
*/
class AssetBase
{
public:
	/**
	* @brief Destructor
	*/
	virtual ~AssetBase() {}

	/**
	* @brief Removes the asset associated with assetID.
	* @param assetID The string ID of the asset.
	*/
	virtual void removeAsset(std::string assetID) = 0;

	/**
	* @brief Fetches the asset of a type with given ID.
	* @param path The path to file to be loaded.
	* @param assocID ID the asset will be associated with
	*/
	virtual void loadAsset(std::string path, std::string assocID) = 0;
};

/**
* @brief Type representing a component storage pool.
* @tparam T The Component to store.
*/
template <typename T>
class AssetPool : public AssetBase
{
public:
	/**
	* @brief Constructor
	*/
	AssetPool() = default;

	/**
	* @brief Destructor.
	*/
	~AssetPool() override {}

	/**
	* @brief Deleted Copy Constructor.
	*/
	AssetPool(const AssetPool&) = delete;

	/**
	* @brief Deleted Move Constructor.
	*/
	AssetPool(AssetPool&&) = delete;

	/**
	* @brief Deleted copy assignment.
	* @return Ref to self.
	*/
	AssetPool& operator=(const AssetPool&) = delete;

	/**
	* @brief Deleted move assignment.
	* @return Ref to self.
	*/
	AssetPool& operator=(AssetPool&&) = delete;

	/**
	* @brief Removes a component associated with entity.
	* @param entHandle Handle to entity
	* @return Void.
	*/
	void	removeAsset(std::string assetID) override;

	/**
	* @brief Gets the component associated with entity.
	* @param entHandle Handle to entity.
	* @return Pointer to component.
	*/
	T*	fetchAsset(std::string assetID);

	/**
	* @brief Creates a component and associates it with the given entity.
	* @tparam Args Types of arguments to forward to component construction.
	* @param entHandle Handle to entity.
	* @param args Arguments to forward to component construction
	* @return Void.
	*/
	void	loadAsset(std::string path, std::string assocID);

private:
	/**
	* @brief Map containing all components of type T.
	*/
	std::map<std::string, T*> _assets{};
};

//=============================================================================
// AssetPool<T>
//=============================================================================

template <typename T>
T* AssetPool<T>::fetchAsset(std::string assetID)
{
	auto it = _assets.find(assetID);

	if (it == _assets.end())
		return nullptr;

	return it->second;
}

template <typename T>
void AssetPool<T>::loadAsset(std::string path, std::string assocID)
{
	char *cstr = new char[path.length() + 1];
	strcpy(cstr, path.c_str());
	_assets.emplace(assocID, new T(cstr));
}

template <typename T>
void AssetPool<T>::removeAsset(std::string assetID)
{
	auto it = _assets.find(assetID);

	if (it == _assets.end())
		return;
	
	delete it->second;
	_assets.erase(it);
}


class AssetManager
{
public:
	//Only default constructor needed
	AssetManager();
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
	~AssetManager() = default;

	template <typename T>
	void load(const std::string& filename, std::string ID);

	template <typename T>
	T* fetch(const std::string& ID);

	template <typename T>
	void dispose(const std::string& ID);

	template <typename T>
	void registerAsset();

	template <typename T>
	AssetPool<T>* getPool();

private:
	std::map<size_t, AssetBase*> _assets{};
};

template <typename T>
void AssetManager::registerAsset()
{
	// Generate a new ID for the component
	size_t id = std::type_index{ typeid(T) }.hash_code();

	auto it = _assets.find(id);

	if (it != _assets.end())
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

