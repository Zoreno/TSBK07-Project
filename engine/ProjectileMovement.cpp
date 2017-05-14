#include "ProjectileMovement.h"
#include "CollisionComponent.h"
#include "TextureComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "ProjectileComponent.h"
#include "CameraComponent.h"


void ProjectileMovement::handleEvent(const KeyEvent & ev)
{
	if (ev.action == 2 && ev.key == GLFW_KEY_2) // 2 is release
	{
		EntityHandle newProjectile = em->createEntity();
		TransformComponent* cameraTransform = em->getComponent<TransformComponent>(1);
		CameraComponent* cameraComponent = em->getComponent<CameraComponent>(1);
		em->assignComponent<TransformComponent>(newProjectile, glm::vec3{ cameraTransform->position });
		em->assignComponent<ModelComponent>(newProjectile, "bunneh");
		em->assignComponent<CollisionComponent>(newProjectile, 1.f);
		em->assignComponent<ProjectileComponent>(newProjectile, 30, cameraComponent->camera.getFrontVector());

	}
}

void ProjectileMovement::handleEvent(const CollisionEvent & ev)
{
	if(em->hasComponent<ProjectileComponent>(ev.entHandle1) || em->hasComponent<ProjectileComponent>(ev.entHandle2))
	{
		em->destroyEntity(ev.entHandle1);
		em->destroyEntity(ev.entHandle2);
	}
}

void ProjectileMovement::startUp()
{
	ev->addSubscriber<CollisionEvent>(this);
	ev->addSubscriber<KeyEvent>(this);
}

void ProjectileMovement::shutDown()
{
	ev->removeSubscriber<CollisionEvent>(this);
	ev->removeSubscriber<KeyEvent>(this);
}

void ProjectileMovement::update(float dt)
{
	// This is hax
	auto updateProjectile = [&](EntityHandle entHandle, TransformComponent* tr, ProjectileComponent* pr)
	{
		tr->position += pr->_direction*pr->_speed*dt;
		pr->_duration += dt;

		if (pr->_duration > 20) em->destroyEntity(entHandle);
	};

	em->each<TransformComponent, ProjectileComponent>(updateProjectile);
}
