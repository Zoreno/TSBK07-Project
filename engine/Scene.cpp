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
	delete EvMptr;
	delete EnMptr;
}

AssetManager* Scene::getAssetManager()
{
	return AMptr;
}

EntityManager* Scene::getEntityManager()
{
	return EnMptr;
}

EventManager* Scene::getEventManager()
{
	return EvMptr;
}
