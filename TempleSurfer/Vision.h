#pragma once

#include "Vision.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include<windows.h>

class Vision
{
public:
	Vision();
	~Vision();

	void visionUpdate();
};

