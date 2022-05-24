#pragma once

#include "Component.h"
#include "FloorComponent.h"
#include <chrono>

class PlayerComponent : public Component
{
	float speed = 5;
	bool jumpOrCrouch = false;
	std::chrono::system_clock::time_point lastTime;

public:
	PlayerComponent();
	~PlayerComponent();

	virtual void update(float elapsedTime) override;
	void moveLeft();
	void moveCenter();
	void moveRight();
	void jump();
	void crouch();
	void resetY();
};

