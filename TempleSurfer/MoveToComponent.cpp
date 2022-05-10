#include "MoveToComponent.h"
#include "GameObject.h"


MoveToComponent::MoveToComponent()
{
}


MoveToComponent::~MoveToComponent()
{
}

void MoveToComponent::update(float elapsedTime)
{
	gameObject->position = (1 - speed) * gameObject->position + speed * target;

	float angle = atan2(gameObject->position.z - target.z, gameObject->position.x - target.x);

	gameObject->rotation.y = 0.05 * angle + 0.95 * gameObject->rotation.y;

}
