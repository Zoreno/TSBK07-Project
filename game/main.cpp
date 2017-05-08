#include <Engine.h>
#include <Scene.h>
#include <Component.h>
#include <System.h>
#include <EntityManager.h>
#include "PointLightComponent.h"
#include "UILabel.h"
#include "TextureComponent.h"
#include "TerrainComponent.h"
#include "MaterialComponent.h"
#include "UIProgressBar.h"

int main()
{
	engine::Engine engine;
	engine.init();

	{
		Scene* testScene = engine.createScene("Test1");

		EntityManager* entityManager = testScene->getEntityManager();
		EventManager* eventManager = testScene->getEventManager();
		userinterface::UIManager* uiManager = testScene->getUIManager();
		AssetManager* assetManager = testScene->getAssetManager();

		//=====================================================================
		// UI setup
		//=====================================================================

		userinterface::UILabel* coordsLabel = uiManager->addElement<userinterface::UILabel>("coords", 320, 32, 0, 1017 - 32);

		coordsLabel->setBorder(4);
		coordsLabel->setBorderColor(Color{ 0.2f });
		coordsLabel->setFillColor(Color{ 0.5f });
		coordsLabel->setTextColor(Color{ 0.f,0.f,0.f });

		userinterface::UILabel* label = uiManager->addElement<userinterface::UILabel>("testRect", 320, 32, 0, 0);

		label->setBorder(4);
		label->setBorderColor(Color{ 0.2f });
		label->setFillColor(Color{ 0.5f });
		label->setTextColor(Color{ 0.f,0.f,0.f });

		userinterface::UIProgressBar* bar = uiManager->addElement<userinterface::UIProgressBar>("testBar", 128, 32, 0, 64, 50.f, 0.f, 100.f);

		bar->setBorder(4);
		bar->setBorderColor(Color{ 0.2f });
		bar->setFillColor(Color{ 0.5f });
		bar->setBarColor(Color{ 1.f,0.f,0.f });
		bar->setTextColor(Color{ 0.f,0.f,0.f });

		//=====================================================================
		// AssetManager setup
		//=====================================================================

		assetManager->registerAsset<TerrainModel>();

		//=====================================================================
		// Load Models
		//=====================================================================

		assetManager->load<RawModel>("bunneh", "../res/models/bunnyplus.obj");
		assetManager->load<RawModel>("tree1", "../res/models/OC12_1.obj");
		//assetManager->load<RawModel>("tree2", "../res/models/OC12_2.obj");
		//assetManager->load<RawModel>("tree3", "../res/models/OC12_3.obj");
		//assetManager->load<RawModel>("tree4", "../res/models/OC12_4.obj");
		//assetManager->load<RawModel>("tree5", "../res/models/OC12_5.obj");
		//assetManager->load<RawModel>("tree6", "../res/models/OC12_6.obj");
		//assetManager->load<RawModel>("tree7", "../res/models/OC12_7.obj");
		//assetManager->load<RawModel>("tree8", "../res/models/OC12_8.obj");
		RawModel* bunModel = assetManager->fetch<RawModel>("bunneh");

		assetManager->registerAsset<Model>();
		Model* mod = LoadModelPlus("../res/models/bunny.obj");
		assetManager->store("bunny", mod);

		assetManager->load<TerrainModel>("basicTerrain", "../res/terrains/fft-terrain.tga");

		//=====================================================================
		// Load textures
		//=====================================================================

		assetManager->load<Texture2D>("grass", "../res/textures/grass.tga");
		assetManager->load<Texture2D>("conc", "../res/textures/conc.tga");
		assetManager->load<Texture2D>("dirt", "../res/textures/dirt.tga");

		//=====================================================================
		// EntityManager setup
		//=====================================================================

		// Görs i scene's konstruktor, registrerar alla saker där

		//=====================================================================
		// Load entities
		//=====================================================================

		EntityHandle terrain = entityManager->createEntity();
		EntityHandle entity2 = entityManager->createEntity();
		EntityHandle entity3 = entityManager->createEntity();
		EntityHandle lightSource1 = entityManager->createEntity();
		EntityHandle lightSource2 = entityManager->createEntity();

		entityManager->assignComponent<TransformComponent>(terrain, glm::vec3{ 0.f,0.f,0.f });
		entityManager->assignComponent<TerrainComponent>(terrain, "basicTerrain");
		entityManager->assignComponent<TextureComponent>(terrain);
		entityManager->assignComponent<MaterialComponent>(terrain, glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 0.2f,0.2f,0.2f }, 64);

		TextureComponent* texComp = entityManager->getComponent<TextureComponent>(terrain);
		texComp->attach(0, "grass");

		entityManager->assignComponent<TransformComponent>(entity2, glm::vec3{ 10.f,1.f,10.f }, glm::radians(270.f), glm::vec3{ 1.f,0.f,0.f });
		entityManager->assignComponent<ModelComponent>(entity2, "tree1");
		entityManager->assignComponent<TextureComponent>(entity2);
		entityManager->assignComponent<CollisionComponent>(entity2, 1.f);

		TextureComponent* texComp2 = entityManager->getComponent<TextureComponent>(entity2);
		texComp2->attach(0, "grass");

		entityManager->assignComponent<TransformComponent>(entity3, glm::vec3{ 92.f,0.5f,165.f });
		entityManager->assignComponent<ModelComponent>(entity3, "bunneh");
		entityManager->assignComponent<CollisionComponent>(entity3, 1.f);
		entityManager->assignComponent<TextureComponent>(entity3);
		entityManager->assignComponent<PointLightComponent>(entity3,
			glm::vec3{ 0.2f, 0.2f, 0.2f },	// Ambient
			glm::vec3{ 26.8f,2.8f,3.3f },	// Diffuse
			glm::vec3{ 1.0f,1.0f,1.0f },	// Specular
			1.f,							// Constant
			0.01f,							// Linear
			0.003f);						// Quadratic

		TextureComponent* texComp3 = entityManager->getComponent<TextureComponent>(entity3);
		texComp3->attach(0, "dirt");


		entityManager->assignComponent<TransformComponent>(lightSource1, glm::vec3{ 10.f, 10.f, 10.f });
		entityManager->assignComponent<PointLightComponent>(lightSource1,
			glm::vec3{ 0.0f, 0.0f, 0.0f },	// Ambient
			glm::vec3{ 0.3f,0.3f,0.3f },	// Diffuse
			glm::vec3{ 1.0f,1.0f,1.0f },	// Specular
			1.f,							// Constant
			0.01f,							// Linear
			0.003f);						// Quadratic


		entityManager->assignComponent<TransformComponent>(lightSource2, glm::vec3{ 92.f, 0.5f, 170.f }, glm::radians(180.f));
		entityManager->assignComponent<ModelComponent>(lightSource2, "bunneh");
		entityManager->assignComponent<PointLightComponent>(lightSource2,
			glm::vec3{ 0.2f, 0.2f, 0.2f },	// Ambient
			glm::vec3{ 2.8f,0.8f,26.3f },	// Diffuse
			glm::vec3{ 1.0f,1.0f,1.0f },	// Specular
			1.f,							// Constant
			0.01f,							// Linear
			0.003f);						// Quadratic
	}

	engine.run();

	engine.cleanup();
}
