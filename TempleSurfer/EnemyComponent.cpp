#include "EnemyComponent.h"
#include <glm/glm.hpp>
#include "GameObject.h"
#include "MoveToComponent.h"

extern std::shared_ptr<GameObject> player;


EnemyComponent::EnemyComponent()
{
}

EnemyComponent::~EnemyComponent()
{
}

void EnemyComponent::update(float elapsedTime)
{
	glm::vec3 difference = gameObject->position - player->position;

	if (glm::length(difference) < 5)
	{
		difference = glm::normalize(difference) * 5.0f;

		gameObject->getComponent<MoveToComponent>()->target = player->position + difference;


	}


}
