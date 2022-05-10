#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class MoveToComponent : public Component
{
public:
	float speed = 0.05f;
	glm::vec3 target;

	MoveToComponent();
	~MoveToComponent();

	virtual void update(float elapsedTime) override;
};

