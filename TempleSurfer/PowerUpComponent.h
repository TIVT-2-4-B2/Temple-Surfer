#pragma once

#include "Component.h"
#include <chrono>
#include <vector>
#include "tigl.h"
using tigl::Vertex;


class PowerUpComponent : public Component
{
private:
	std::vector<Vertex> verts;
	std::chrono::system_clock::time_point lastTime;
	void powerupIntersect(glm::vec3 playerHitbox, glm::vec3 playerPosition, const glm::vec3& parentMatrix);
	glm::vec3 makePositive(glm::vec3 vector);
	glm::vec3 powerHitbox;
public:
	

	PowerUpComponent(glm::vec3 powerHitbox);
	~PowerUpComponent();

	virtual void update(float elapsedTime, const glm::vec3& parentMatrix) override;

};
