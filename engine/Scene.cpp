#include "Scene.h"
#include <stdio.h>
#include "TerrainComponent.h"
#include "TextureComponent.h"
#include "PointLightComponent.h"
#include "MaterialComponent.h"
#include "ProjectileMovement.h"
#include "ProjectileComponent.h"
#include <ctime>


Scene::Scene(AssetManager* AM, Window* window) :
	asM{ AM },
	enM{ nullptr },
	evM{ nullptr },
	uiM{ nullptr }
{
	evM = new EventManager{};
	uiM = new userinterface::UIManager(window->getWidth(), window->getHeight());
	enM = new EntityManager{ evM, asM, uiM };
	quadtree = new Quadtree{enM, evM, glm::vec2{100, 100}, 300, 300};

	evM->addSubscriber<CollisionEvent>(this);
	evM->addSubscriber<KeyEvent>(this);

	enM->registerComponent<CollisionComponent>("CollisionComponent");
	enM->registerComponent<TransformComponent>("TransformComponent");
	enM->registerComponent<ModelComponent>("ModelComponent");
	enM->registerComponent<CameraComponent>("CameraComponent");
	enM->registerComponent<QuadtreeComponent>("QuadtreeComponent");
	enM->registerComponent<TerrainComponent>("TerrainComponent");
	enM->registerComponent<TextureComponent>("TextureComponent");
	enM->registerComponent<PointLightComponent>("PointLightComponent");
	enM->registerComponent<MaterialComponent>("MaterialComponent");
	enM->registerComponent<ProjectileComponent>("ProjectileComponent");

	// Detta tar hand om instansiering och sånt.
	enM->registerSystem<CameraController>();
	enM->registerSystem<RenderingSystem>(window);
	enM->registerSystem<ProjectileMovement>();
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

void Scene::handleEvent(const KeyEvent & ev)
{
	if (!(ev.action == 2 && ev.key == GLFW_KEY_F)) return;

	int max1 = 130; int min1 = 80; int range1 = max1 - min1 + 1;
	int max2 = 180; int min2 = 120; int range2 = max2 - min2 + 1;
	int max3 = 4; int min3 = 1; int range3 = max3 - min3 + 1;
	srand(time(NULL));
	int num = rand() % range1 + min1;
	EntityHandle tree1 = enM->createEntity();
	enM->assignComponent<TransformComponent>(tree1, glm::vec3{ float(rand() % range1 + min1),0.0f,float(rand() % range2 + min2) }, glm::radians(270.f), glm::vec3{ 1.f,0.f,0.f });
	enM->assignComponent<ModelComponent>(tree1, std::string("tree").append(std::to_string(rand() % range3 + min3)));
	enM->assignComponent<TextureComponent>(tree1);
	enM->assignComponent<CollisionComponent>(tree1, 1.f);
	TextureComponent* texComp2 = enM->getComponent<TextureComponent>(tree1);
	texComp2->attach(0, "grass");
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
