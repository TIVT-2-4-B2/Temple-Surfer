#pragma once

#include "CollisionComponent.h"
#include <chrono>


class PowerUpComponent : public CollisionComponent
{
private:
	std::vector<Vertex> verts;
	std::chrono::system_clock::time_point lastTime;

public:
	glm::vec3 powerHitbox;

	PowerUpComponent(glm::vec3 powerHitbox);
	~PowerUpComponent();

	void intersect(glm::vec3 playerHitbox, glm::vec3 playerPosition, const glm::vec3& parentMatrix);
	virtual void update(float elapsedTime, const glm::vec3& parentMatrix) override;
	virtual void draw() override;
};
