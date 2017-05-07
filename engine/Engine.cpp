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

namespace engine
{
	void Engine::init()
	{
		window = new Window{ 800, 600, "MyWindow" };

		dumpInfo(std::cout);

		text = new TextRenderer{ "../res/fonts/arial.ttf" };

		assetManager = new AssetManager{};
		Scene* testScene = createScene("Test1");

		EntityManager* entityManager = testScene->getEntityManager();
		EventManager* eventManager = testScene->getEventManager();

		assetManager->load<RawModel>("bunneh", "../res/models/bunny.obj");
		RawModel* bunModel = assetManager->fetch<RawModel>("bunneh");

		assetManager->registerAsset<Model>();
		Model* mod = LoadModelPlus("../res/models/bunny.obj");
		assetManager->store("bunny", mod);

		EntityHandle entity2 = entityManager->createEntity();
		EntityHandle entity3 = entityManager->createEntity();

		entityManager->assignComponent<TransformComponent>(entity2, glm::vec3{ 5.f,0.f,0.f }, 45.f);
		entityManager->assignComponent<ModelComponent>(entity2, "bunneh");

		entityManager->assignComponent<TransformComponent>(entity3, glm::vec3{ 10.f,0.f,0.f });
		entityManager->assignComponent<ModelComponent>(entity3, "bunneh");

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
				}
				break;
				case EventType::MOUSE_MOVED_EVENT:
				{
					MouseEvent new_event;
					new_event.posX = ev.mouse.posx;
					new_event.posY = ev.mouse.posy;

					currentScene->getEventManager()->postEvent(new_event);
				}
				break;
				case EventType::GAINED_FOCUS:
				{
					window->setCursorMode(CursorMode::HIDDEN);
				}
				break;
				case EventType::LOST_FOCUS:
				{
					window->setCursorMode(CursorMode::NORMAL);
				}
				break;
				default:
				{}
				break;
				}
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			currentScene->getEntityManager()->update(static_cast<float>(timeDelta));

			GLfloat tickTime = dutyTimer.reset();

			char buf[100];
			sprintf(buf, "%i FPS, TimeDelta: %f, CPU: %.0f%%", fps, timeDelta, 100.f*tickTime/timeDelta);

			text->render(buf, 15.f, 570.f, 0.3f, Color{ 0.5f, 0.8f, 0.2f });

			window->display();
		}
	}

	void Engine::cleanup()
	{
		delete text;

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

		Scene* scenePtr = new Scene{ assetManager };

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
