/**
 * @file	AssetManager.h
 * @Author	Kevin Kjellén
 * @date	2017-05-20
 * @brief	Asset Manager Interface
 */

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
/**
 * \brief asset manager error class
 */
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
	~AssetPool() override {for (auto& i : _assets) { delete i.second; }}

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
	* @brief Removes an asset associated with the ID.
	* @param assetID ID to asset
	* @return Void.
	*/
	void	removeAsset(std::string assetID) override;

	/**
	* @brief Gets the component associated with entity.
	* @param assetID ID to asset.
	* @return Pointer to component.
	*/
	T*	fetchAsset(std::string assetID);

	/**
	* @brief Loads the asset with args as constructor arguments and with assocID as key.
	* @param assocID ID the asset will be associated with
	* @param args Arguments to be passed on to the constructor
	*/
	template <typename ... Args>
	void	loadAsset(std::string assocID, Args ... args);

	/**
	* @brief Store the asset with the ID as key.
	* @param assocID ID the asset will be associated with
	* @param assetPtr Pointer to an asset
	*/
	void	storeAsset(std::string assocID, T* assetPtr);

	/**
	* @brief Store the asset with the ID as key.
	* @param assocID ID the asset will be associated with
	* @param assetRef Reference to an asset
	*/
	void	storeAsset(std::string assocID, T& assetRef);

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
template <typename ... Args>
void AssetPool<T>::loadAsset(std::string assocID, Args ... args)
{
	_assets.emplace(assocID, new T(std::forward<Args>(args)...));
}

template <typename T>
void AssetPool<T>::storeAsset(std::string assocID, T* assetPtr)
{
	_assets.emplace(assocID, assetPtr);
}

template <typename T>
void AssetPool<T>::storeAsset(std::string assocID, T& assetRef)
{
	_assets.emplace(assocID, &assetRef);
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
	~AssetManager();

	/**
	* @brief Fetches the asset of a type with given ID.
	* @param ID ID the asset will be associated with
	* @param args Arguments to be passed on to the constructor
	*/
	template <typename T, typename ... Args>
	void load(std::string ID, Args ... args);

	/**
	* @brief Store the asset with the ID as key.
	* @param assocID ID the asset will be associated with
	* @param assetPtr Reference to an asset
	*/
	template <typename T>
	void store(const std::string& assocID, T* assetPtr);

	/**
	* @brief Store the asset with the ID as key.
	* @param assocID ID the asset will be associated with
	* @param assetRef Reference to an asset
	*/
	template <typename T>
	void store(const std::string& assocID, T& assetRef);

	/**
	* @brief Gets the component associated with entity.
	* @param ID ID to asset.
	* @return Pointer to component.
	*/
	template <typename T>
	T* fetch(const std::string& ID);

	/**
	* @brief Removes an asset associated with the ID.
	* @param ID ID to asset
	* @return Void.
	*/
	template <typename T>
	void dispose(const std::string& ID);

	/**
	 * @brief Registers a type to the asset map. Needed to be able to store things.
	 * @return void
	 */
	template <typename T>
	void registerAsset();

	/**
	* @brief Returns the pool holding all assets of type T.
	* @return Pointer to pool
	*/
	template <typename T>
	AssetPool<T>* getPool();

private:
	/**
	* @brief Pool holding all the individual asset pools.
	*/
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

template <typename T, typename ... Args>
void AssetManager::load(std::string ID, Args ... args)
{
	AssetPool<T>* pool = getPool<T>();

	pool->loadAsset(ID, std::forward<Args>(args)...);
}

template <typename T>
void AssetManager::store(const std::string& assocID, T* assetPtr)
{
	AssetPool<T>* pool = getPool<T>();

	pool->storeAsset(assocID, assetPtr);
}
template <typename T>
void AssetManager::store(const std::string& assocID, T& assetRef)
{
	AssetPool<T>* pool = getPool<T>();

	pool->storeAsset(assocID, assetRef);
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

