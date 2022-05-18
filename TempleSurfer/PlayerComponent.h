#pragma once

#include "Component.h"
#include "FloorComponent.h"

class PlayerComponent : public Component
{
	float speed = 5;
public:
	PlayerComponent();
	~PlayerComponent();

	virtual void update(float elapsedTime) override;
	void moveLeft();
	void moveCenter();
	void moveRight();
};

