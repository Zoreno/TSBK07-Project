#include "ProjectileMovement.h"
#include "CollisionComponent.h"
#include "TextureComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "ProjectileComponent.h"
#include "CameraComponent.h"
#include "PointLightComponent.h"
#include <ctime>
#include "MaterialComponent.h"


void ProjectileMovement::handleEvent(const KeyEvent & ev)
{
	if (ev.action == 2 && ev.key == GLFW_KEY_2) // 2 is release
	{
		srand(time(NULL));
		EntityHandle newProjectile = em->createEntity();
		TransformComponent* cameraTransform = em->getComponent<TransformComponent>(1);
		CameraComponent* cameraComponent = em->getComponent<CameraComponent>(1);
		em->assignComponent<TransformComponent>(newProjectile, glm::vec3{ cameraTransform->position });
		em->assignComponent<ModelComponent>(newProjectile, "bunneh");
		em->assignComponent<CollisionComponent>(newProjectile, 1.f);
		em->assignComponent<ProjectileComponent>(newProjectile, 60, cameraComponent->camera.getFrontVector());
		em->assignComponent<MaterialComponent>(newProjectile, glm::vec3{ 10.f,1.f,1.f }, glm::vec3{ 10.f,1.f,1.f }, glm::vec3{1.f,1.f,1.f}, 64);
		em->assignComponent<PointLightComponent>(newProjectile,
			glm::vec3{ 0.0f, 0.0f, 0.0f },	// Ambient
			glm::vec3{ ((float)rand() / (RAND_MAX))*2,((float)rand() / (RAND_MAX))*2,((float)rand() / (RAND_MAX))*2 },	// Diffuse
			glm::vec3{ 1.0f,1.0f,1.0f },	// Specular
			1.f,							// Constant
			0.01f,							// Linear
			0.003f);						// Quadratic

	}
}

void ProjectileMovement::handleEvent(const CollisionEvent & ev)
{
	if(em->hasComponent<ProjectileComponent>(ev.entHandle1) || em->hasComponent<ProjectileComponent>(ev.entHandle2))
	{
		TransformComponent* trans1 = em->getComponent<TransformComponent>(ev.entHandle1);
		TransformComponent* trans2 = em->getComponent<TransformComponent>(ev.entHandle2);
		if (std::abs(trans1->position.y - trans2->position.y) < 2)
		{
			em->destroyEntity(ev.entHandle1);
			em->destroyEntity(ev.entHandle2);
		}
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

		if (pr->_duration > 5) em->destroyEntity(entHandle);
	};

	em->each<TransformComponent, ProjectileComponent>(updateProjectile);
}
