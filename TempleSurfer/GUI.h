#pragma once

#include "Vision.h"

class GUI
{

private:
	int WindowWidth;
	int WindowHeight;

	void showScore(std::vector<int> digits);
	std::vector<int> intToDigits(int number);
	void GenerateImageTexture(const std::string& fileName);
	void BindImageTexture();
	void GenerateTexture(cv::Mat& cameraImage);
	void BindTexture();

public:

	GUI(int WindowWidth, int WindowHeight);
	~GUI();

	void drawMenu();
	void drawGUI(std::shared_ptr<Vision> vision);
	void showPowerupIcon();

};

