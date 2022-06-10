#pragma once

#include "DrawComponent.h"
#include <glm/glm.hpp>
#include <vector>
#include "tigl.h"
using tigl::Vertex;

class CollisionComponent : public DrawComponent
{
protected:
    glm::vec3 makePositive(glm::vec3 vector);

private:
    std::vector<Vertex> verts;

public:
    glm::vec3 hitbox;
    bool powerup = false;

    CollisionComponent();
	CollisionComponent(glm::vec3 hitbox);
	~CollisionComponent();

	void intersect(glm::vec3 playerHitbox, glm::vec3 playerPosition, const glm::vec3& parentMatrix);
    virtual void update(float elapsedTime, const glm::vec3& parentMatrix) override;
    virtual void draw() override;
};