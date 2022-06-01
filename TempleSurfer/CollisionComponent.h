#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class CollisionComponent : public DrawComponent
{
private:
    std::vector<Vertex> verts;

    glm::vec3 makePositive(glm::vec3 vector);

public:
    glm::vec3 hitbox;

	CollisionComponent(glm::vec3 hitbox);
	~CollisionComponent();

	void intersect(glm::vec3 playerHitbox, glm::vec3 playerPosition, const glm::vec3& parentMatrix);
    virtual void update(float elapsedTime, const glm::vec3& parentMatrix) override;
    virtual void draw() override;
};