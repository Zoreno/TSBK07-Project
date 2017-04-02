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
		explicit TransformComponent(glm::vec3 position, float angle = 0.f, glm::vec3 rotationAxis = glm::vec3{ 0.f,1.f,0.f }, glm::vec3 scale = glm::vec3{ 1.f,1.f,1.f })
			: position(position), angle(angle), rotationAxis(rotationAxis), scale(scale) {}

		glm::vec3 position{};
		float angle{};
		glm::vec3 rotationAxis{};
		glm::vec3 scale{};

		TransformPipeline3D getPipeline() const
		{
			TransformPipeline3D pipeline{};

			pipeline.translate(position);

			if(angle != 0.f)
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

	void Engine::init()
	{
		window = new Window{ 800, 600, "MyWindow" };

		dumpInfo(std::cout);

		text = new TextRenderer{ "../res/fonts/arial.ttf" };

		em = new EntityManager{};

		em->registerComponent<TransformComponent>();
		em->registerComponent<NameComponent>();

		EntityHandle entity1 = em->createEntity();
		EntityHandle entity2 = em->createEntity();

		em->assignComponent<TransformComponent>(entity1, glm::vec3{ 1.f,0.f,0.f });
		em->assignComponent<TransformComponent>(entity2, glm::vec3{ 3.f,0.f,0.f });
		em->assignComponent<NameComponent>(entity1, "Bös...");
		//em->assignComponent<NameComponent>(entity2, "Hello Test");

		auto func = [](EntityHandle ent, TransformComponent* transform)
		{
			std::cout << "Hej" << std::endl;

			std::cout << transform->position << std::endl;
		};

		em->each<TransformComponent>(func);

		auto func2 = [](EntityHandle ent, TransformComponent* tr, NameComponent* na)
		{
			std::cout << "HejHej" << std::endl;

			std::cout << tr->position << std::endl;

			std::cout << na->userString << std::endl;
		};

		em->each<TransformComponent, NameComponent>(func2);
	}

	void Engine::run()
	{
		while (!window->shouldClose())
		{
			WindowEvent ev;
			while (window->pollEvent(ev));

			text->render("Hello World!", 25.f, 25.f, 0.5f, Color{ 0.5f, 0.8f, 0.2f });

			window->display();
		}
	}

	void Engine::cleanup()
	{
		delete text;

		delete window;

		delete em;
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
