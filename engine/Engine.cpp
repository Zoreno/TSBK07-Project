#include "Engine.h"

#include "RawModel.h"

#include <iostream>

#include "TransformPipeline3D.h"

#include "ComponentContainer.h"
#include "Entity.h"
#include "Component.h"

namespace engine
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent() {}

		glm::vec3 position;
	};

	void Engine::init()
	{
		window = new Window{ 800, 600, "MyWindow" };

		dumpInfo(std::cout);

		text = new TextRenderer{ "../res/fonts/arial.ttf" };

		Entity ent;

		TransformComponent trans{};

		trans.position = glm::vec3{ 4.f,0.f,0.f };

		ComponentContainer::addComponent(ent.getID(), trans);
	}

	void Engine::run()
	{
		while (!window->shouldClose())
		{
			text->render("Hello World!", 25.f, 25.f, 0.5f, Color{ 0.5f, 0.8f, 0.2f });

			window->display();

			std::cout << ComponentContainer::getComponent<TransformComponent>(0)->position.x << std::endl;
		}
	}

	void Engine::cleanup()
	{
		delete text;

		delete window;
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
