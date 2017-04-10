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

		eventManager = new EventManager{};

		assetManager = new AssetManager{};

		assetManager->load<RawModel>("bunneh", "../res/models/bunny.obj");
		RawModel* bunModel = assetManager->fetch<RawModel>("bunneh");

		assetManager->registerAsset<Model>();
		Model* mod = LoadModelPlus("../res/models/bunny.obj");
		assetManager->store("bunny", mod);

		entityManager = new EntityManager{ eventManager , assetManager };

		entityManager->registerComponent<TransformComponent>("TransformComponent");
		entityManager->registerComponent<ModelComponent>("ModelComponent");
		entityManager->registerComponent<CameraComponent>("CameraComponent");

		EntityHandle entity1 = entityManager->createEntity();
		EntityHandle entity2 = entityManager->createEntity();

		entityManager->assignComponent<TransformComponent>(entity1, glm::vec3{ 0.f,0.f,0.f });
		entityManager->assignComponent<CameraComponent>(entity1);

		entityManager->assignComponent<TransformComponent>(entity2, glm::vec3{ 5.f,0.f,0.f });
		entityManager->assignComponent<ModelComponent>(entity2, "bunneh");

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
			GLfloat timeDelta = timer.reset();

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

					eventManager->postEvent(new_event);
				}
				break;
				case EventType::MOUSE_MOVED_EVENT:
				{
					MouseEvent new_event;
					new_event.posX = ev.mouse.posx;
					new_event.posY = ev.mouse.posy;

					eventManager->postEvent(new_event);
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

			char buf[100];
			sprintf(buf, "%i FPS, TimeDelta: %f", fps, timeDelta);

			entityManager->update(static_cast<float>(timeDelta));

			text->render(buf, 15.f, 570.f, 0.3f, Color{ 0.5f, 0.8f, 0.2f });

			window->display();
		}
	}

	void Engine::cleanup()
	{
		delete text;

		delete window;

		delete entityManager;

		delete eventManager;

		delete assetManager;
	}

	EntityManager* Engine::getEntityManager() const
	{
		return entityManager;
	}

	EventManager* Engine::geteventManager() const
	{
		return eventManager;
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
