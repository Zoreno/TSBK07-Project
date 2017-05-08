#include "Engine.h"

#include "RawModel.h"

#include <iostream>
#include <sstream>

#include "TransformPipeline3D.h"

#include "Component.h"
#include "EntityManager.h"
#include "AssetManager.h"

#include "TransformComponent.h"
#include "Utils.h"

#include <rapidxml/rapidxml.hpp>
#include "ModelComponent.h"
#include "RenderingSystem.h"
#include "CameraComponent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "CameraController.h"
#include "UILabel.h"
#include "UIProgressBar.h"
#include "TerrainModel.h"
#include "TerrainComponent.h"
#include "TextureComponent.h"
#include "PointLightComponent.h"
#include "MaterialComponent.h"

namespace engine
{
	void Engine::init()
	{
		WindowSettings settings = getDefaultWindowSettings();

		settings.maximized = true;

		window = new Window{ 1920, 1080, "MyWindow" , settings };

		dumpInfo(std::cout);

		assetManager = new AssetManager{};
		Scene* testScene = createScene("Test1");

		EntityManager* entityManager = testScene->getEntityManager();
		EventManager* eventManager = testScene->getEventManager();
		userinterface::UIManager* uiManager = testScene->getUIManager();

		//=====================================================================
		// UI setup
		//=====================================================================

		userinterface::UILabel* coordsLabel = uiManager->addElement<userinterface::UILabel>("coords", 320, 32, 0, 1017-32);

		coordsLabel->setBorder(4);
		coordsLabel->setBorderColor(Color{ 0.2f });
		coordsLabel->setFillColor(Color{ 0.5f});
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

		entityManager->assignComponent<TransformComponent>(entity2, glm::vec3{ 10.f,1.f,10.f }, glm::radians(270.f), glm::vec3{1.f,0.f,0.f});
		entityManager->assignComponent<ModelComponent>(entity2, "tree1");
		entityManager->assignComponent<TextureComponent>(entity2);
		entityManager->assignComponent<CollisionComponent>(entity2, 1.f);

		TextureComponent* texComp2 = entityManager->getComponent<TextureComponent>(entity2);
		texComp2->attach(0, "grass");

		entityManager->assignComponent<TransformComponent>(entity3, glm::vec3{ 19.f,12.5f,117.f });
		entityManager->assignComponent<ModelComponent>(entity3, "bunneh");
		entityManager->assignComponent<CollisionComponent>(entity3, 1.f);
		entityManager->assignComponent<TextureComponent>(entity3);

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
			
		
		entityManager->assignComponent<TransformComponent>(lightSource2, glm::vec3{ 19.f, 15.f, 111.f });
		entityManager->assignComponent<PointLightComponent>(lightSource2,
			glm::vec3{ 0.2f, 0.2f, 0.2f },	// Ambient
			glm::vec3{ 6.8f,6.8f,3.3f },	// Diffuse
			glm::vec3{ 1.0f,1.0f,1.0f },	// Specular
			1.f,							// Constant
			0.01f,							// Linear
			0.003f);						// Quadratic

		// Detta tar hand om instansiering och sånt.
		entityManager->registerSystem<CameraController>();
		entityManager->registerSystem<RenderingSystem>(window);

		window->setCursorMode(CursorMode::DISABLED);
	}

	void Engine::run()
	{
		int frames{ 0 };
		int fps{ 0 };

		GLfloat timeElapsed{ 0.f };

		while (!window->shouldClose())
		{
			Scene* currentScene = Scenes.find(activeScene)->second;
			userinterface::UIManager* uiManager = currentScene->getUIManager();

			currentScene->update();
			GLfloat timeDelta = timer.reset();
			Timer dutyTimer{};

			timeElapsed += timeDelta;
			frames++;
			if (timeElapsed > 1.f)
			{
				timeElapsed -= 1.f;
				fps = frames;
				frames = 0;
			}

			WindowEvent ev;
			while (window->pollEvent(ev))
			{
				switch (ev.type)
				{
				case EventType::KEY_EVENT:
				{
					KeyEvent new_event;
					new_event.key = ev.key.key;
					new_event.action = (int)ev.key.action;

					currentScene->getEventManager()->postEvent(new_event);

					// Just for testing
					// TODO: Remove
					if (ev.key.key == GLFW_KEY_0 && ev.key.action == Action::PRESS)
						uiManager->getElement<userinterface::UILabel>("testRect")->toggleVisibility();

					if (ev.key.key == GLFW_KEY_1 && ev.key.action == Action::PRESS)
						uiManager->getElement<userinterface::UIProgressBar>("testBar")->decrementValue(10.f);

					if (ev.key.key == GLFW_KEY_2 && ev.key.action == Action::PRESS)
						uiManager->getElement<userinterface::UIProgressBar>("testBar")->incrementValue(10.f);
				}
				break;
				case EventType::MOUSE_MOVED_EVENT:
				{
					MouseEvent new_event;
					new_event.posX = ev.mouse.posx;
					new_event.posY = ev.mouse.posy;

					currentScene->getEventManager()->postEvent(new_event);

					//std::cout << ev.mouse.posx << " " << ev.mouse.posy << std::endl;
				}
				break;
				case EventType::MOUSE_KEY_EVENT:
				{
					if(ev.mouse.button == GLFW_MOUSE_BUTTON_1 && ev.mouse.action == Action::PRESS)
						uiManager->getElement<userinterface::UIProgressBar>("testBar")->decrementValue(10.f);

					if (ev.mouse.button == GLFW_MOUSE_BUTTON_2 && ev.mouse.action == Action::PRESS)
						uiManager->getElement<userinterface::UIProgressBar>("testBar")->incrementValue(10.f);
				}
				break;
				case EventType::GAINED_FOCUS:
				{
					window->setCursorMode(CursorMode::DISABLED);
				}
				break;
				case EventType::LOST_FOCUS:
				{
					window->setCursorMode(CursorMode::NORMAL);
				}
				break;
				case EventType::RESIZED:
				{
					//text->setScreenDimensions(ev.size.width, ev.size.height);
				}
				break;
				default:
				{}
				break;
				}
			}

			currentScene->getEntityManager()->update(static_cast<float>(timeDelta));

			GLfloat tickTime = dutyTimer.reset();

			char buf[100];
			sprintf(buf, "%i FPS, TimeDelta: %f, CPU: %.1f%%", fps, timeDelta, 100.f*tickTime / timeDelta);

			uiManager->getElement<userinterface::UILabel>("testRect")->setText(buf);

			glDisable(GL_DEPTH_TEST);

			uiManager->draw();

			glEnable(GL_DEPTH_TEST);

			window->display();
		}
	}

	void Engine::cleanup()
	{
		delete window;

		for (auto& i : Scenes) delete i.second;

		delete assetManager;
	}

	Scene* Engine::createScene(std::string ID)
	{
		auto sc = Scenes.find(ID);
		if (sc != Scenes.end())
			throw Engine_error(std::string("Scene ID '").append(ID).append("' already exist"));

		if (assetManager == nullptr)
			throw Engine_error("Cannot create scene. AssetManager is uninitialized");

		Scene* scenePtr = new Scene{ assetManager, window };

		Scenes.emplace(ID, scenePtr);

		if (activeScene == "") activeScene = ID;

		return scenePtr;
	}

	Scene * Engine::getScene(std::string ID) const
	{
		auto sc = Scenes.find(ID);

		if (sc == Scenes.end())
			throw Engine_error(std::string("Scene ID '").append(ID).append("' not found"));

		return sc->second;
	}

	void Engine::setActiveScene(std::string sceneID)
	{
		if(Scenes.find(sceneID) == Scenes.end())

		activeScene = sceneID;
	}

	EntityManager* Engine::getEntityManager(std::string sceneID) const
	{
		auto sc = Scenes.find(sceneID);

		if (sc == Scenes.end())
			throw Engine_error(std::string("Scene ID '").append(sceneID).append("' not found"));

		return sc->second->getEntityManager();
	}

	EventManager* Engine::getEventManager(std::string sceneID) const
	{
		auto sc = Scenes.find(sceneID);

		if (sc == Scenes.end())
			throw Engine_error(std::string("Scene ID '").append(sceneID).append("' not found"));

		return sc->second->getEventManager();
	}

	AssetManager* Engine::getAssetManager() const
	{
		return assetManager;
	}

	void Engine::dumpInfo(std::ostream& stream)
	{
		bool listExtensions = false;

		int majorVersion;
		int minorVersion;

		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		stream << "OpenGL Version: " << majorVersion << "." << minorVersion;

		const char* str;

		if (majorVersion >= 3)
		{
			str = reinterpret_cast<const char*>(glGetString(GL_VERSION));

			stream << " (" << str << ")";
		}

		stream << std::endl;

		str = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

		stream << "Vendor: " << str << std::endl;

		str = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

		stream << "Renderer: " << str << std::endl;

		str = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

		stream << "GLSL Version: " << str << std::endl;

		int numExtensions;

		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

		stream << "Number of extensions: " << numExtensions << std::endl;

		if (!listExtensions)
			return;

		for (int i{ 0 }; i < numExtensions; ++i)
		{
			str = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));

			stream << "Extension " << i << ": " << str << std::endl;
		}
	}
}
