#include <Engine.h>

#include <ComponentContainer.h>
#include <Component.h>
#include <Entity.h>
#include <iostream>

class UserComponent : public Component
{
public:
	std::string userString;
};

int main()
{
	engine::Engine engine;

	engine.init();

	Entity userEntity;

	UserComponent userComponent;

	userComponent.userString = "Hej jag heter kalle";

	ComponentContainer::addComponent(userEntity.getID(), userComponent);

	engine.run();

	std::cout << ComponentContainer::getComponent<UserComponent>(userEntity.getID())->userString << std::endl;

	engine.cleanup();
}