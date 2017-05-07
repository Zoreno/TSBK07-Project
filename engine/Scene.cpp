#include "Scene.h"
#include <stdio.h>


Scene::Scene(AssetManager* AM) :
AMptr{AM},
EnMptr{nullptr},
EvMptr{nullptr}
{
	EvMptr = new EventManager{};
	EnMptr = new EntityManager{EvMptr, AMptr};
	quadtree = new Quadtree{EnMptr, glm::vec2{0, 0}, 100, 100};

	EnMptr->registerComponent<CollisionComponent>("CollisionComponent");
	EnMptr->registerComponent<TransformComponent>("TransformComponent");
	EnMptr->registerComponent<ModelComponent>("ModelComponent");
	EnMptr->registerComponent<CameraComponent>("CameraComponent");
	EnMptr->registerComponent<QuadtreeComponent>("QuadtreeComponent");

	EntityHandle entity1 = EnMptr->createEntity();

	EnMptr->assignComponent<TransformComponent>(entity1, glm::vec3{ 0.f,0.f,0.f }, 45.f);

	EntityHandle entity2 = EnMptr->createEntity();

	EnMptr->assignComponent<TransformComponent>(entity2, glm::vec3{ 30.f,30.f,30.f }, 0.f);
	EnMptr->assignComponent<CollisionComponent>(entity2, 1.f);

	quadtree->pushEntity(entity1);
	quadtree->pushEntity(entity2);

	std::cout << "Top layer: " << quadtree->getEntCount() << std::endl;
	std::cout << "Total: " << quadtree->getTotalEntCount() << std::endl;
	std::cout << "Position: " << EnMptr->getComponent<QuadtreeComponent>(entity2)->getPosition() << std::endl;

	quadtree->update();
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

void Scene::update()
{
}
