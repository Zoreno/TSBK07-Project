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

class Scene
{
public:
	Scene() = delete;
	explicit Scene(AssetManager* AsM);
	~Scene();

	AssetManager* getAssetManager() const;
	EntityManager* getEntityManager() const;
	EventManager* getEventManager() const;

	void update();
private:
	Quadtree* quadtree;
	AssetManager* AMptr;
	EntityManager* EnMptr;
	EventManager* EvMptr;
};

