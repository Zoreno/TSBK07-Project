#include "Scene.h"
#include <stdio.h>


Scene::Scene(AssetManager* AM) :
AMptr{AM},
enM{nullptr},
evM{nullptr}
{
	evM = new EventManager{};
	enM = new EntityManager{evM, AMptr};
	quadtree = new Quadtree{enM, evM, glm::vec2{0, 0}, 100, 100};

	enM->registerComponent<CollisionComponent>("CollisionComponent");
	enM->registerComponent<TransformComponent>("TransformComponent");
	enM->registerComponent<ModelComponent>("ModelComponent");
	enM->registerComponent<CameraComponent>("CameraComponent");
	enM->registerComponent<QuadtreeComponent>("QuadtreeComponent");

	EntityHandle entity1 = enM->createEntity();

	enM->assignComponent<TransformComponent>(entity1, glm::vec3{ 0.f,0.f,0.f }, 45.f);

	EntityHandle entity2 = enM->createEntity();

	enM->assignComponent<TransformComponent>(entity2, glm::vec3{ 30.f,30.f,30.f }, 0.f);
	enM->assignComponent<CollisionComponent>(entity2, 1.f);

	//quadtree->pushEntity(entity1);
	//quadtree->pushEntity(entity2);

	std::cout << "Top layer: " << quadtree->getEntCount() << std::endl;
	std::cout << "Total: " << quadtree->getTotalEntCount() << std::endl;
	std::cout << "Position: " << enM->getComponent<QuadtreeComponent>(entity2)->getPosition() << std::endl;

	enM->destroyEntity(entity1);
	std::cout << "Top layer: " << quadtree->getEntCount() << std::endl;
	std::cout << "Total: " << quadtree->getTotalEntCount() << std::endl;

	quadtree->update();
}


Scene::~Scene()
{
	delete enM;
	delete evM;
}

AssetManager* Scene::getAssetManager() const
{
	return AMptr;
}

EntityManager* Scene::getEntityManager() const
{
	return enM;
}

EventManager* Scene::getEventManager() const
{
	return evM;
}

void Scene::update()
{
}
