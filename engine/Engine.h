#pragma once

#include "EngineDLL.h"

#include "Window.h"

#include "Color.h"

#include "TextRenderer.h"

#include "EntityManager.h"

#include "AssetManager.h"

namespace engine
{
	class ENGINE_API Engine
	{
	public:
		Engine() = default;
		~Engine() {}

		void init();

		void run();

		void cleanup();

		EntityManager* getEntityManager() const;
		EventManager* geteventManager() const;
		AssetManager* getAssetManager() const;

	private:

		void dumpInfo(std::ostream& stream);

		Window* window{ nullptr };

		TextRenderer* text{ nullptr };

		EntityManager* entityManager{};
		EventManager* eventManager{};
		AssetManager* assetManager{};
	};
}
