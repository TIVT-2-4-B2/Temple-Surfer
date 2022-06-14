#pragma once

#include "Vision.h"

class GUI
{

private:
	int WindowWidth;
	int WindowHeight;

	void showScore(std::vector<int> digits);
	std::vector<int> intToDigits(int number);
	void GenerateDigitTexture(const std::string& fileName);
	void GenerateCameraTexture(cv::Mat& cameraImage);
	void GeneratePowerUpTexture(const std::string& fileName);
	void BindTexture(GLuint id);

public:

	GUI(int WindowWidth, int WindowHeight);
	~GUI();

	void drawMenu();
	void drawGUI(std::shared_ptr<Vision> vision);
	void showPowerupIcon();

};

