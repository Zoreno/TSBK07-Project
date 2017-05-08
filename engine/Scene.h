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

class Scene : public Subscriber<CollisionEvent>
{
public:
	Scene() = delete;
	explicit Scene(AssetManager* AsM);
	~Scene();

	void handleEvent(const CollisionEvent& ev) override;

	AssetManager* getAssetManager() const;
	EntityManager* getEntityManager() const;
	EventManager* getEventManager() const;

	void update();
private:
	Quadtree* quadtree;
	AssetManager* AMptr;
	EntityManager* enM;
	EventManager* evM;
};

