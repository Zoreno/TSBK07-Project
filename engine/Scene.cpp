#include "Scene.h"
#include <stdio.h>
#include "TerrainComponent.h"
#include "TextureComponent.h"
#include "PointLightComponent.h"
#include "MaterialComponent.h"


Scene::Scene(AssetManager* AM, Window* window) :
asM{AM},
enM{nullptr},
evM{nullptr},
uiM{nullptr}
{
	evM = new EventManager{};
	uiM = new userinterface::UIManager(window->getWidth(), window->getHeight());
	enM = new EntityManager{ evM, asM, uiM };
	quadtree = new Quadtree{enM, evM, glm::vec2{100, 100}, 300, 300};

	evM->addSubscriber<CollisionEvent>(this);

	enM->registerComponent<CollisionComponent>("CollisionComponent");
	enM->registerComponent<TransformComponent>("TransformComponent");
	enM->registerComponent<ModelComponent>("ModelComponent");
	enM->registerComponent<CameraComponent>("CameraComponent");
	enM->registerComponent<QuadtreeComponent>("QuadtreeComponent");
	enM->registerComponent<TerrainComponent>("TerrainComponent");
	enM->registerComponent<TextureComponent>("TextureComponent");
	enM->registerComponent<PointLightComponent>("PointLightComponent");
	enM->registerComponent<MaterialComponent>("MaterialComponent");

	// Detta tar hand om instansiering och sånt.
	enM->registerSystem<CameraController>();
	enM->registerSystem<RenderingSystem>(window);
}

Scene::~Scene()
{
	delete enM;
	delete quadtree;
	delete evM;
	delete uiM;
}

void Scene::handleEvent(const CollisionEvent& ev)
{

}

AssetManager* Scene::getAssetManager() const
{
	return asM;
}

EntityManager* Scene::getEntityManager() const
{
	return enM;
}

EventManager* Scene::getEventManager() const
{
	return evM;
}

userinterface::UIManager * Scene::getUIManager() const
{
	return uiM;;
}

void Scene::update()
{
	quadtree->update();
}
