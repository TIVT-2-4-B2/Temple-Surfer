#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class CollisionComponent : public Component
{
private:
    glm::vec3 makePositive(glm::vec3 vector);

public:
    glm::vec3 hitbox;

	CollisionComponent(glm::vec3 hitbox);
	~CollisionComponent();

	void intersect(glm::vec3 playerHitbox, glm::vec3 playerPosition);
    virtual void update(float elapsedTime) override;
};