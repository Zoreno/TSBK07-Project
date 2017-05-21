#pragma once

#include "AssetManager.h"
#include "EntityManager.h"
#include "Quadtree.h"

#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "QuadtreeComponent.h"
#include "Utils.h"

#include "TransformPipeline3D.h"

#include <rapidxml/rapidxml.hpp>
#include "ModelComponent.h"
#include "RenderingSystem.h"
#include "CameraComponent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "CameraController.h"
#include "CollisionEvent.h"

/**
 * \brief Scene class, to be held by the engine
 */
class Scene : public Subscriber<CollisionEvent>, public Subscriber<KeyEvent>
{
public:
	Scene() = delete;
	/**
	 * \brief Constructor of a scene
	 * \param AsM Pointer to the global asset manager
	 * \param window pointer to the window
	 */
	explicit Scene(AssetManager* AsM, Window* window);
	~Scene();

	/**
	 * \brief Event handler for collision events
	 * \param ev Event to be handled
	 */
	void handleEvent(const CollisionEvent& ev) override;

	/**
	 * \brief Event handler for keyboard events 
	 * \param ev Event to be handled
	 */
	void handleEvent(const KeyEvent& ev) override;

	/**
	 * \brief Gets asset manager
	 * \return return pointer to the asset manager
	 */
	AssetManager* getAssetManager() const;

	/**
	 * \brief Gets the entity manager
	 * \return return pointer to the entity manager
	 */
	EntityManager* getEntityManager() const;

	/**
	 * \brief Gets the event manager
	 * \return return pointer to the event manager
	 */
	EventManager* getEventManager() const;

	/**
	 * \brief Gets the ui manager
	 * \return return poitner to the ui manager
	 */
	userinterface::UIManager* getUIManager() const;

	/**
	 * \brief Updates the scene
	 */
	void update();
private:

	/**
	 * \brief Pointer to the quadtree of the scene
	 */
	Quadtree* quadtree;

	/**
	 * \brief pointer to the global asset manager
	 */
	AssetManager* asM;

	/**
	 * \brief pointer to the scenes entity manager
	 */
	EntityManager* enM;

	/**
	 * \brief pointer to the scenes event manager
	 */
	EventManager* evM;

	/**
	 * \brief Pointer to the scenes ui manager
	 */
	userinterface::UIManager* uiM;
};

