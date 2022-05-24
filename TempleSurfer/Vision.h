#pragma once

#include "Vision.h"

#include "PlayerComponent.h"

class Vision
{
private:	
	enum xPosition
	{
		LEFT = 0,
		CENTER = 1,
		RIGHT = 2
	};

	xPosition xPos = CENTER;

public:
	Vision();
	Vision(std::shared_ptr<PlayerComponent> playerComponent);
	~Vision();

	void visionUpdate();
};

