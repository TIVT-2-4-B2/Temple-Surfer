#pragma once

#include "Component.h"

class TimerJumper : public Component
{
	float timer = 0;
	float targetX = -5;
public:
	TimerJumper();
	~TimerJumper();

	virtual void update(float elapsedTime) override;
};

