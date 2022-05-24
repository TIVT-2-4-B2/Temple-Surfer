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

	enum yPosition
	{
		JUMP = 0,
		STAND = 1,
		DUCK = 2
	};

	xPosition xPos = CENTER;
	yPosition yPos = STAND;

public:
	Vision();
	Vision(std::shared_ptr<PlayerComponent> playerComponent);
	~Vision();

	void visionUpdate();
};

