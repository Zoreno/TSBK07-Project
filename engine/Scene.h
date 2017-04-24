#pragma once

#include "AssetManager.h"
#include "EntityManager.h"

#include "TransformComponent.h"
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

	AssetManager* getAssetManager();
	EntityManager* getEntityManager();
	EventManager* getEventManager();
private:
	AssetManager* AMptr;
	EntityManager* EnMptr;
	EventManager* EvMptr;
};

