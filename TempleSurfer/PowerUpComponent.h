#pragma once

#include "CollisionComponent.h"


class PowerUpComponent : public CollisionComponent
{
public:
	glm::vec3 powerHitbox;

	PowerUpComponent(glm::vec3 powerHitbox);
	~PowerUpComponent();

	void intersect(glm::vec3 playerHitbox, glm::vec3 playerPosition, const glm::vec3& parentMatrix);
	virtual void update(float elapsedTime, const glm::vec3& parentMatrix) override;
	virtual void draw() override;
};
