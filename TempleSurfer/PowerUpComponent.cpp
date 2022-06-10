#include "PowerUpComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
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
		lastTime = std::chrono::system_clock::now();
		player->getComponent<CollisionComponent>()->powerup = true;
		std::cout << "Engaged" << std::endl;
		//To Do: Remove the powerup component on pick-up.
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

const int powerupDuration = 10;

void PowerUpComponent::update(float elapsedTime, const glm::vec3& parentMatrix) {
	if (player != nullptr && player->getComponent<CollisionComponent>() != nullptr)
	{
		powerupIntersect(player->getComponent<CollisionComponent>()->hitbox, player->position, parentMatrix);
	}
	if (player->getComponent<CollisionComponent>()->powerup) {
		std::chrono::system_clock::duration duration = std::chrono::system_clock::now() - lastTime;
		std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds> (duration);
		std::cout << seconds << std::endl;
		if (seconds.count() > powerupDuration)
		{
			std::cout << "Disengaged" << std::endl;
			player->getComponent<CollisionComponent>()->powerup = false;
		}
	}
}