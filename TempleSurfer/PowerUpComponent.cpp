#include "PowerUpComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include <Vector>
#include <memory>
#include <iostream>

extern std::shared_ptr<GameObject> player;

PowerUpComponent::PowerUpComponent(glm::vec3 powerHitbox)
{
	CollisionComponent::CollisionComponent(powerHitbox);
}

PowerUpComponent::~PowerUpComponent() 
{
}

void PowerUpComponent::intersect(glm::vec3 playerHitbox, glm::vec3 playerPosition, const glm::vec3& parentMatrix) {
	if (this->gameObject->position == playerPosition && hitbox == playerHitbox)
	{
		return;
	}
	glm::vec3 gamePos = parentMatrix + this->gameObject->position;
	glm::vec3 posDif = makePositive(gamePos - playerPosition);
	glm::vec3 hitBoxSum = makePositive((hitbox / this->gameObject->scale) + playerHitbox);
	if (posDif.x <= hitBoxSum.x && posDif.y <= hitBoxSum.y && posDif.z <= hitBoxSum.z)
	{
		player->getComponent<CollisionComponent>()->powerup = true;
		lastTime = std::chrono::system_clock::now();
	}
}

const int powerupDuration = 8;

void PowerUpComponent::update(float elapsedTime, const glm::vec3& parentMatrix) {
	if (powerup) {
		std::chrono::system_clock::duration duration = std::chrono::system_clock::now() - lastTime;
		std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds> (duration);
		if (seconds.count() > powerupDuration)
		{
			player->getComponent<CollisionComponent>()->powerup = false;
		}
	}
}

void PowerUpComponent::draw() {

}