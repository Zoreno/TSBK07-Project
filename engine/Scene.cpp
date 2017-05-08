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

	evM->addSubscriber<CollisionEvent>(this);

	enM->registerComponent<CollisionComponent>("CollisionComponent");
	enM->registerComponent<TransformComponent>("TransformComponent");
	enM->registerComponent<ModelComponent>("ModelComponent");
	enM->registerComponent<CameraComponent>("CameraComponent");
	enM->registerComponent<QuadtreeComponent>("QuadtreeComponent");
}

Scene::~Scene()
{
	delete enM;
	delete evM;
}

void Scene::handleEvent(const CollisionEvent& ev)
{
	std::cout << "Collision!!" << ev.entHandle1 << ", " << ev.entHandle2 << std::endl;
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
	quadtree->update();
}
