#include "PowerUpComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "OBJComponent.h"
#include "CollisionComponent.h"
#include "CubeComponent.h"
#include <Vector>
#include <memory>
#include <iostream>

extern std::shared_ptr<GameObject> player;

PowerUpComponent::PowerUpComponent(glm::vec3 powerHitbox) : powerHitbox(powerHitbox)
{
}

PowerUpComponent::~PowerUpComponent()
{
	std::cout << "Forced Disengage" << std::endl;
	player->getComponent<CollisionComponent>()->powerup = false;
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
		if (player->getComponent<CollisionComponent>()->powerup == false || (seconds != std::chrono::seconds::max() && seconds.count() < 3)) {
			lastTime = std::chrono::system_clock::now();
			std::cout << "Engaged" << std::endl;
		}
		player->getComponent<CollisionComponent>()->powerup = true;
		gameObject->removeComponent(gameObject->getComponent<OBJComponent>());
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

const int powerupDuration = 12;

void PowerUpComponent::update(float elapsedTime, const glm::vec3& parentMatrix) {
	if (player != nullptr && player->getComponent<CollisionComponent>() != nullptr)
	{
		powerupIntersect(player->getComponent<CollisionComponent>()->hitbox, player->position, parentMatrix);
	}
	if (player->getComponent<CollisionComponent>()->powerup) {
		std::chrono::system_clock::duration duration = std::chrono::system_clock::now() - lastTime;
		seconds = std::chrono::duration_cast<std::chrono::seconds> (duration);
		std::cout << seconds << std::endl;
		if (seconds.count() > powerupDuration && seconds.count() < powerupDuration * 2)
		{
			std::cout << "Disengaged" << std::endl;
			player->getComponent<CollisionComponent>()->powerup = false;
			seconds = std::chrono::seconds::max();
		}
	}
}