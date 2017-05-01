#include "Scene.h"



Scene::Scene(AssetManager* AM) :
AMptr{AM},
EnMptr{nullptr},
EvMptr{nullptr}
{
	EvMptr = new EventManager{};
	EnMptr = new EntityManager{EvMptr, AMptr};
}


Scene::~Scene()
{
	delete EnMptr;
	delete EvMptr;
}

AssetManager* Scene::getAssetManager() const
{
	return AMptr;
}

EntityManager* Scene::getEntityManager() const
{
	return EnMptr;
}

EventManager* Scene::getEventManager() const
{
	return EvMptr;
}
