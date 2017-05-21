/**
 * @file	Engine.h
 * @Author	Joakim Bertils
 * @date	2017-05-21
 * @brief	Engine
 */

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

/**
 * @brief Map Containing game scenes.
 */
typedef std::map<std::string, Scene*> SceneMap;

namespace engine
{
	/**
	 * @brief Engine error class
	 */
	class Engine_error : public std::logic_error {
		using std::logic_error::logic_error;
	};

	/**
	 * @brief Engine class
	 * 
	 */
	class ENGINE_API Engine
	{
	public:
		/**
		 * @brief Constructor
		 */
		Engine() = default;

		/**
		 * @brief Destructor
		 */
		~Engine() {}

		/**
		 * @brief Initiates the engine
		 */
		void init();

		/**
		 * @brief Runs the main loop
		 */
		void run();

		/**
		 * @brief Cleanup function
		 */
		void cleanup();

		/**
		 * @brief Creates a scene
		 * @param ID Identifier
		 * @return Pointer to created scene
		 */
		Scene* createScene(const std::string& ID);

		/**
		 * @brief Gets a scene from the engine.
		 * @param ID Identifier
		 * @return Pointer to scene
		 */
		Scene* getScene(const std::string& ID) const;

		/**
		 * @brief Sets a scene as active
		 * @param ID Identifier
		 */
		void setActiveScene(const std::string& ID);

		/**
		 * @brief Gets an entity manager associated with a scene
		 * @param sceneID Identifier
		 * @return Pointer to entity manager
		 */
		EntityManager* getEntityManager(const std::string& sceneID) const;

		/**
		* @brief Gets an event manager associated with a scene
		* @param sceneID Identifier
		* @return Pointer to event manager
		*/
		EventManager* getEventManager(const std::string& sceneID) const;

		/**
		* @brief Gets the asset manager
		* @return Pointer to asset manager
		*/
		AssetManager* getAssetManager() const;

	private:

		/**
		 * @brief Dumps OpenGL info
		 * @param stream Stream to use
		 */
		void dumpInfo(std::ostream& stream);

		/**
		 * @brief Pointer to window
		 */
		Window* window{ nullptr };

		/**
		 * @brief All scenes in the engine
		 */
		SceneMap Scenes;

		/**
		 * @brief The currently active scene
		 */
		std::string activeScene;

		/**
		 * @brief Asset Manager pointer
		 */
		AssetManager* assetManager{nullptr};

		/**
		 * @brief Timer to provide timestep info.
		 */
		Timer timer{};
	};
}
