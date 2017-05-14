#include "Engine.h"
#include "UILabel.h"
#include "UIProgressBar.h"
#include "TerrainModel.h"
#include "PointLightComponent.h"
#include "TextureComponent.h"
#include "MaterialComponent.h"
#include "TerrainComponent.h"
#include <random>
#include <filesystem>
void createSomeTrees(EntityManager* entityManager);

int main()
{
	engine::Engine engine;

	engine.init();

	Scene* testScene = engine.createScene("Test1");

	{
		AssetManager* assetManager = testScene->getAssetManager();
		EntityManager* entityManager = testScene->getEntityManager();
		EventManager* eventManager = testScene->getEventManager();
		userinterface::UIManager* uiManager = testScene->getUIManager();

		//=====================================================================
		// UI setup
		//=====================================================================
		{
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
		}
		//=====================================================================
		// AssetManager setup
		//=====================================================================
		{
			assetManager->registerAsset<TerrainModel>();
		}
		//=====================================================================
		// Load Models
		//=====================================================================
		{
			assetManager->load<RawModel>("bunneh", "../res/models/bunnyplus.obj");
			assetManager->load<RawModel>("tree1", "../res/models/OC12_1.obj");
			assetManager->load<RawModel>("tree2", "../res/models/OC12_2.obj");
			assetManager->load<RawModel>("tree3", "../res/models/OC12_3.obj");
			assetManager->load<RawModel>("tree4", "../res/models/OC12_4.obj");
			assetManager->load<RawModel>("tree5", "../res/models/OC12_5.obj");
			assetManager->load<RawModel>("tree6", "../res/models/OC12_6.obj");
			assetManager->load<RawModel>("tree7", "../res/models/OC12_7.obj");
			assetManager->load<RawModel>("tree8", "../res/models/OC12_8.obj");
			RawModel* bunModel = assetManager->fetch<RawModel>("bunneh");

			assetManager->registerAsset<Model>();
			Model* mod = LoadModelPlus("../res/models/bunny.obj");
			assetManager->store("bunny", mod);

			assetManager->load<TerrainModel>("basicTerrain", "../res/terrains/fft-terrain.tga");
		}
		//=====================================================================
		// Load textures
		//=====================================================================

		assetManager->load<Texture2D>("grass", "../res/textures/GrassGreenTexture0003.tga");
		assetManager->load<Texture2D>("conc", "../res/textures/conc.tga");
		assetManager->load<Texture2D>("dirt", "../res/textures/dirt.tga");

		//=====================================================================
		// EntityManager setup
		//=====================================================================

		// Görs i scene's konstruktor, registrerar alla saker där

		//=====================================================================
		// Load entities
		//=====================================================================
		{
			EntityHandle terrain = entityManager->createEntity();
			
			EntityHandle entity3 = entityManager->createEntity();
			EntityHandle lightSource1 = entityManager->createEntity();
			EntityHandle lightSource2 = entityManager->createEntity();

			entityManager->assignComponent<TransformComponent>(terrain, glm::vec3{ 0.f,0.f,0.f });
			entityManager->assignComponent<TerrainComponent>(terrain, "basicTerrain");
			entityManager->assignComponent<TextureComponent>(terrain);
			entityManager->assignComponent<MaterialComponent>(terrain, glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 0.2f,0.2f,0.2f }, 64);

			TextureComponent* texComp = entityManager->getComponent<TextureComponent>(terrain);
			texComp->attach(0, "grass");

			createSomeTrees(entityManager);

			entityManager->assignComponent<TransformComponent>(entity3, glm::vec3{ 92.f,0.5f,165.f });
			entityManager->assignComponent<ModelComponent>(entity3, "bunneh");
			entityManager->assignComponent<CollisionComponent>(entity3, 1.f);
			entityManager->assignComponent<TextureComponent>(entity3);
			/*entityManager->assignComponent<PointLightComponent>(entity3,
				glm::vec3{ 0.2f, 0.2f, 0.2f },	// Ambient
				glm::vec3{ 26.8f,2.8f,3.3f },	// Diffuse
				glm::vec3{ 1.0f,1.0f,1.0f },	// Specular
				1.f,							// Constant
				0.01f,							// Linear
				0.003f);						// Quadratic
				*/
			TextureComponent* texComp3 = entityManager->getComponent<TextureComponent>(entity3);
			texComp3->attach(0, "dirt");


			entityManager->assignComponent<TransformComponent>(lightSource1, glm::vec3{ 10.f, 10.f, 10.f });
			//entityManager->assignComponent<ModelComponent>(lightSource1, "bunneh");
			entityManager->assignComponent<PointLightComponent>(lightSource1,
				glm::vec3{ 0.0f, 0.0f, 0.0f },	// Ambient
				glm::vec3{ 0.3f,0.3f,0.3f },	// Diffuse
				glm::vec3{ 1.0f,1.0f,1.0f },	// Specular
				1.f,							// Constant
				0.01f,							// Linear
				0.003f);						// Quadratic


			entityManager->assignComponent<TransformComponent>(lightSource2, glm::vec3{ 92.f, 20.f, 170.f }, glm::radians(180.f));
			entityManager->assignComponent<ModelComponent>(lightSource2, "bunneh");
			entityManager->assignComponent<PointLightComponent>(lightSource2,
				glm::vec3{ 0.3f, 0.3f, 0.2f },	// Ambient
				glm::vec3{ 3.8f,3.8f,1.3f },	// Diffuse
				glm::vec3{ 1.0f,1.0f,1.0f },	// Specular
				1.f,							// Constant
				0.01f,							// Linear
				0.003f);						// Quadratic
		}
	}
	std::cout << (7 >> 3) << std::endl;
	engine.run();

	engine.cleanup();
}

void createSomeTrees(EntityManager* entityManager)
{
	int max1 = 130; int min1 = 80; int range1 = max1 - min1 + 1;
	int max2 = 180; int min2 = 120; int range2 = max2 - min2 + 1;
	int max3 = 8; int min3 = 1; int range3 = max3 - min3 + 1;
	int num = rand() % range1 + min1;
	for (int i = 0; i < 20; ++i)
	{
		EntityHandle tree1 = entityManager->createEntity();
		entityManager->assignComponent<TransformComponent>(tree1, glm::vec3{ float(rand() % range1 + min1),0.0f,float(rand() % range2 + min2) }, glm::radians(270.f), glm::vec3{ 1.f,0.f,0.f });
		entityManager->assignComponent<ModelComponent>(tree1, std::string("tree").append(std::to_string(rand()  % range3 + min3)));
		entityManager->assignComponent<TextureComponent>(tree1);
		entityManager->assignComponent<CollisionComponent>(tree1, 1.f);
		TextureComponent* texComp2 = entityManager->getComponent<TextureComponent>(tree1);
		texComp2->attach(0, "grass");
	}
}