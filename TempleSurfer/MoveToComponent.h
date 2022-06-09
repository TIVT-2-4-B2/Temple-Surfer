#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class MoveToComponent : public Component
{
public:
	float speed = 0.5f;
	glm::vec3 target;

	MoveToComponent(glm::vec3 target);
	~MoveToComponent();

	virtual void update(float elapsedTime) override;
};

