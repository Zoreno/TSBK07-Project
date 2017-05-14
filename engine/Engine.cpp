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

		//settings.maximized = true;

		window = new Window{ 1280, 768, "MyWindow" , settings };

		dumpInfo(std::cout);

		assetManager = new AssetManager{};

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
