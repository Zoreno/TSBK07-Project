#include "Engine.h"

#include "RawModel.h"

#include <iostream>

#include "TransformPipeline3D.h"

#include "Component.h"
#include "EntityManager.h"

std::ostream& operator<<(std::ostream& os, glm::vec3 vec)
{
	return os << '{' << vec.x << ',' << vec.y << ',' << vec.z << '}';
}

namespace engine
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent() = default;
		explicit TransformComponent(glm::vec3 position = glm::vec3{ 0.f,0.f,0.f }, float angle = 0.f, glm::vec3 rotationAxis = glm::vec3{ 0.f,1.f,0.f }, glm::vec3 scale = glm::vec3{ 1.f,1.f,1.f })
			: position(position), angle(angle), rotationAxis(rotationAxis), scale(scale) {}

		glm::vec3 position{};
		float angle{};
		glm::vec3 rotationAxis{};
		glm::vec3 scale{};

		TransformPipeline3D getPipeline() const
		{
			TransformPipeline3D pipeline{};

			pipeline.translate(position);

			if (angle != 0.f)
			{
				pipeline.rotate(angle, rotationAxis);
			}
			pipeline.scale(scale);

			return pipeline;
		}
	};

	class NameComponent : public Component
	{
	public:
		NameComponent() = default;
		explicit NameComponent(const std::string& str) : userString(str) {}


		std::string userString{};
	};

	class TestEvent : public Event
	{
	public:
		std::string data{};
	};

	class KeyEvent : public Event
	{
	public:
		int key;
	};

	class NameSystem :
		public System,
		public Subscriber<TestEvent>,
		public Subscriber<EntityCreatedEvent>,
		public Subscriber<EntityDestroyedEvent>,
		public Subscriber<KeyEvent>
	{
	public:
		NameSystem() = default;

		void handleEvent(const TestEvent& ev) override
		{
			std::cout << ev.data << std::endl;
		}

		void handleEvent(const EntityCreatedEvent& ev) override
		{
			std::cout << "Entity Created: " << ev.entHandle << std::endl;
		}

		void handleEvent(const EntityDestroyedEvent& ev) override
		{
			std::cout << "Entity Destroyed: " << ev.entHandle << std::endl;
		}

		void handleEvent(const KeyEvent& ev) override
		{
			EntityHandle ent = em->createEntity();

			em->assignComponent<TransformComponent>(ent, glm::vec3{ 1.f,0.f,0.f });
		}

		void startUp() override
		{
			ev->addSubscriber<TestEvent>(this);
			ev->addSubscriber<EntityCreatedEvent>(this);
			ev->addSubscriber<EntityDestroyedEvent>(this);
			ev->addSubscriber<KeyEvent>(this);
		}

		void shutDown() override
		{
			ev->removeSubscriber<TestEvent>(this);
			ev->removeSubscriber<EntityCreatedEvent>(this);
			ev->removeSubscriber<EntityDestroyedEvent>(this);
			ev->removeSubscriber<KeyEvent>(this);
		}

		void update(float dt) override
		{
			auto func = [this](EntityHandle ent, TransformComponent* tr) -> void
			{
				tr->position += glm::vec3{ 1.f, 0.f,0.f };

				std::cout << ent << " " << tr->position << std::endl;

				if (tr->position.x > 10.f)
				{
					em->destroyEntity(ent);
				}
			};

			em->each<TransformComponent>(func); // Lambda func

		}
	};

	void Engine::init()
	{
		window = new Window{ 800, 600, "MyWindow" };

		dumpInfo(std::cout);

		text = new TextRenderer{ "../res/fonts/arial.ttf" };

		eventManager = new EventManager{};

		entityManager = new EntityManager{ eventManager };

		entityManager->registerComponent<TransformComponent>();
		entityManager->registerComponent<NameComponent>();

		EntityHandle entity1 = entityManager->createEntity();
		EntityHandle entity2 = entityManager->createEntity();

		entityManager->assignComponent<TransformComponent>(entity1, glm::vec3{ 1.f,0.f,0.f });
		entityManager->assignComponent<TransformComponent>(entity2, glm::vec3{ 3.f,0.f,0.f });
		entityManager->assignComponent<NameComponent>(entity1, "Bös...");
		entityManager->assignComponent<NameComponent>(entity2, "Hello Test");

		// Detta tar hand om instansiering och sånt.
		entityManager->registerSystem<NameSystem>();

		TestEvent eve;
		eve.data = "TestData";

		eventManager->postEvent(eve);
	}

	void Engine::run()
	{
		while (!window->shouldClose())
		{
			WindowEvent ev;
			while (window->pollEvent(ev))
			{
				if (ev.type == EventType::KEY_EVENT)
				{
					if (ev.key.action == Action::PRESS)
					{
						std::cout << "Key Press event" << std::endl;

						KeyEvent new_event;
						new_event.key = ev.key.key;

						eventManager->postEvent(new_event);
					}
				}
			}

			text->render("Hello World!", 25.f, 25.f, 0.5f, Color{ 0.5f, 0.8f, 0.2f });

			entityManager->update(0);

			window->display();
		}
	}

	void Engine::cleanup()
	{
		delete text;

		delete window;

		delete entityManager;

		delete eventManager;
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
