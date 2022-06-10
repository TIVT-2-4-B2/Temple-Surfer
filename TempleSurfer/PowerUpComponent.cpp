#include "PowerUpComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "CollisionComponent.h"
#include <Vector>
#include <memory>
#include <iostream>

extern std::shared_ptr<GameObject> player;

PowerUpComponent::PowerUpComponent(glm::vec3 powerHitbox) : powerHitbox(powerHitbox)
{
}

PowerUpComponent::~PowerUpComponent() 
{
}

void PowerUpComponent::powerupIntersect(glm::vec3 playerHitbox, glm::vec3 playerPosition, const glm::vec3& parentMatrix) {
	if (this->gameObject->position == playerPosition && powerHitbox == playerHitbox)
	{
		return;
	}
	glm::vec3 gamePos = parentMatrix + this->gameObject->position;
	glm::vec3 posDif = makePositive(gamePos - playerPosition);
	glm::vec3 hitBoxSum = makePositive((powerHitbox / this->gameObject->scale) + playerHitbox);
	if (posDif.x <= hitBoxSum.x && posDif.y <= hitBoxSum.y && posDif.z <= hitBoxSum.z)
	{
		std::cout << "Powerup Engaged" << std::endl;
		player->getComponent<CollisionComponent>()->powerup = true;
		lastTime = std::chrono::system_clock::now();
	}
}

glm::vec3 PowerUpComponent::makePositive(glm::vec3 vector)
{
	if (vector.x < 0)
	{
		vector.x = -vector.x;
	}
	if (vector.y < 0)
	{
		vector.y = -vector.y;
	}
	if (vector.z < 0)
	{
		vector.z = -vector.z;
	}
	return vector;
}

const int powerupDuration = 8;

void PowerUpComponent::update(float elapsedTime, const glm::vec3& parentMatrix) {
	if (player != nullptr && player->getComponent<CollisionComponent>() != nullptr)
	{
		powerupIntersect(player->getComponent<CollisionComponent>()->hitbox, player->position, parentMatrix);
	}
	if (player->getComponent<CollisionComponent>()->powerup) {
		std::chrono::system_clock::duration duration = std::chrono::system_clock::now() - lastTime;
		std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds> (duration);
		std::cout << seconds.count() << std::endl;
		if (seconds.count() > powerupDuration)
		{
			std::cout << "Powerup Disengaged" << std::endl;
			player->getComponent<CollisionComponent>()->powerup = false;
		}
	}
}