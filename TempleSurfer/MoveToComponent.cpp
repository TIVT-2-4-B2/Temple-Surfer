#include "MoveToComponent.h"
#include "GameObject.h"


MoveToComponent::MoveToComponent(glm::vec3 target) : target(target)
{
}


MoveToComponent::~MoveToComponent()
{
}

void MoveToComponent::update(float elapsedTime)
{
	gameObject->position = (1 - speed) * gameObject->position + speed * target;
}
