#pragma once

#include <map>

#include "EngineDLL.h"

#include "Window.h"

#include "Color.h"

#include "Scene.h"
#include "EntityManager.h"

#include "AssetManager.h"

#include "UIManager.h"
#include "Timer.h"

typedef std::map<std::string, Scene*> SceneMap;

namespace engine
{
	class Engine_error : public std::logic_error {
		using std::logic_error::logic_error;
	};

	class ENGINE_API Engine
	{
	public:
		Engine() = default;
		~Engine() {}

		void init();

		void run();

		void cleanup();

		Scene* createScene(std::string);
		Scene* getScene(std::string) const;
		void setActiveScene(std::string);
		EntityManager* getEntityManager(std::string sceneID) const;
		EventManager* getEventManager(std::string sceneID) const;
		AssetManager* getAssetManager() const;

	private:

		void dumpInfo(std::ostream& stream);

		Window* window{ nullptr };

		SceneMap Scenes;
		std::string activeScene;
		AssetManager* assetManager{nullptr};

		Timer timer{};
	};
}
