#pragma once

#include "Vision.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "PlayerComponent.h"
#include <queue>
#include <mutex>

class Vision
{
private:	
	// Creating enums for position
	enum class xPosition
	{
		LEFT,
		CENTER,
		RIGHT
	};
	enum class yPosition
	{
		JUMP,
		STAND,
		DUCK
	};

	// Setting standard positions
	xPosition xPos = xPosition::CENTER;
	yPosition yPos = yPosition::STAND;

	// For crossthread communication
	std::queue<xPosition> xInputQueue;
	std::queue<yPosition> yInputQueue;
	std::mutex lockInputQueues;

	// Private methods
	void visionRoutine();

	// Sub divided methods that check other things
	void checkForFaces();
	void debugWindow();
	void checkResult();

public:
	Vision(std::shared_ptr<PlayerComponent> playerComponent);
	~Vision();

	cv::Mat getImage();
	void visionUpdate();
};

