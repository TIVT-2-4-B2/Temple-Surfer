#include "GUI.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "tigl.h"
#include "Vision.h"

#include "GameObject.h"
#include "CollisionComponent.h"

//v for width to convert
//w for width
//h for height
//Swap width and height if value is height instead of width
#define ToAspect(v, w, h) (v/w*h)

GLuint digitID;
GLuint cameraID;
GLuint powerupID;
int textureCount = 0;

extern std::shared_ptr<GameObject> player;

const int textureInterval = 5;

const int digitWidth = 60;
const int digitHeight = 80;
const int digitOffsetX = 20;
const int digitOffsetY = 20;

const int powerIconWidth = 120;
const int powerIconHeight = 240;

const float cameraScale = 0.16f;

std::vector<Vertex> menuVerts;
std::vector<Vertex> cameraVerts;
std::vector<Vertex> powerupVerts;

GUI::GUI(int WindowWidth, int WindowHeight)
{
	this->WindowWidth = WindowWidth;
	this->WindowHeight = WindowHeight;

	GeneratePowerUpTexture("models/powerupicon.png");

	menuVerts.push_back(Vertex::PT(glm::vec3(0, WindowHeight, 0), glm::vec2(0, 1)));
	menuVerts.push_back(Vertex::PT(glm::vec3(WindowWidth, WindowHeight, 0), glm::vec2(1, 1)));
	menuVerts.push_back(Vertex::PT(glm::vec3(WindowWidth, 0, 0), glm::vec2(1, 0)));
	menuVerts.push_back(Vertex::PT(glm::vec3(0, 0, 0), glm::vec2(0, 0)));

	cameraVerts.push_back(Vertex::PT(glm::vec3(0, ToAspect(WindowWidth / (1 / cameraScale), 4, 3), 0), glm::vec2(0, 1)));
	cameraVerts.push_back(Vertex::PT(glm::vec3((WindowWidth / (1 / cameraScale)), ToAspect(WindowWidth / (1 / cameraScale), 4, 3), 0), glm::vec2(1, 1)));
	cameraVerts.push_back(Vertex::PT(glm::vec3((WindowWidth / (1 / cameraScale)), 0, 0), glm::vec2(1, 0)));
	cameraVerts.push_back(Vertex::PT(glm::vec3(0, 0, 0), glm::vec2(0, 0)));

	powerupVerts.push_back(Vertex::PCTN(glm::vec3(WindowWidth / 2 - powerIconWidth, 0, 0), glm::vec4(0, 1, 1, 0.7f), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	powerupVerts.push_back(Vertex::PCTN(glm::vec3(WindowWidth / 2 + powerIconWidth, 0, 0), glm::vec4(0, 1, 1, 0.7f), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	powerupVerts.push_back(Vertex::PCTN(glm::vec3(WindowWidth / 2 + powerIconWidth, powerIconHeight, 0), glm::vec4(0, 1, 1, 0.7f), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	powerupVerts.push_back(Vertex::PCTN(glm::vec3(WindowWidth / 2 - powerIconWidth, powerIconHeight, 0), glm::vec4(0, 1, 1, 0.7f), glm::vec2(0, 1), glm::vec3(0, 1, 0)));

}

GUI::~GUI()
{
}

void GUI::drawMenu() {
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tigl::shader->enableFog(false);
	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);

	glm::mat4 projection = glm::ortho(0.0f, (float)WindowWidth, (float)WindowHeight, 0.0f, -1.0f, 1.0f);
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableTexture(true);
	tigl::shader->enableColor(false);

	std::string fileName = "Resources\\Menu.png";
	GenerateDigitTexture(fileName);
	BindTexture(digitID);

	glDisable(GL_DEPTH_TEST);

	//Draw menu verts
	tigl::drawVertices(GL_QUADS, menuVerts);

	glEnable(GL_DEPTH_TEST);

	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);

}

void GUI::drawGUI(std::shared_ptr<Vision> vision) {

	extern int score;

	tigl::shader->enableFog(false);

	//Draw UI
	glm::mat4 projection = glm::ortho(0.0f, (float)WindowWidth, (float)WindowHeight, 0.0f, -1.0f, 1.0f);
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableTexture(true);
	tigl::shader->enableColor(false);

	//Get image from vision
	cv::Mat img = vision->getImage();

	//Load image as texture
	if (textureCount % textureInterval == 0)
	{
		GenerateCameraTexture(img);
	}
	else {
		BindTexture(cameraID);
	}
	textureCount++;

	glDisable(GL_DEPTH_TEST);

	//Draw camera verts
	tigl::drawVertices(GL_QUADS, cameraVerts);

	std::vector digits = intToDigits(score);
	showScore(digits);

	if (player->getComponent<CollisionComponent>()->powerup == true) {
		showPowerupIcon();
	}

	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);

	glEnable(GL_DEPTH_TEST);

}

void GUI::showScore(std::vector<int> digits) {

	for (int i = (digits.size() - 1); i >= 0; i--)
	{
		std::string fileName;
		fileName.append("Resources\\digit");
		fileName.append(std::to_string(digits.at(i)));
		fileName.append(".png");

		GenerateDigitTexture(fileName);
		BindTexture(digitID);

		int digitX = (WindowWidth - (i * digitWidth)) - digitOffsetX;

		tigl::begin(GL_QUADS);
		tigl::addVertex(Vertex::PT(glm::vec3(digitX - digitWidth, digitOffsetY + digitHeight, 0), glm::vec2(0, 1)));
		tigl::addVertex(Vertex::PT(glm::vec3(digitX, digitOffsetY + digitHeight, 0), glm::vec2(1, 1)));
		tigl::addVertex(Vertex::PT(glm::vec3(digitX, digitOffsetY, 0), glm::vec2(1, 0)));
		tigl::addVertex(Vertex::PT(glm::vec3(digitX - digitWidth, digitOffsetY, 0), glm::vec2(0, 0)));
		tigl::end();

	}

}

std::vector<int> GUI::intToDigits(int number) {
	std::vector<int> digits;

	while (number > 0) {
		digits.push_back(number % 10);
		number = number / 10;
	}

	return digits;
}



void GUI::GeneratePowerUpTexture(const std::string& fileName) {
	int width, height, bpp;
	stbi_uc* data = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

	glDeleteTextures(1, &powerupID);
	glGenTextures(1, &powerupID);
	BindTexture(powerupID);
	glTexImage2D(GL_TEXTURE_2D,
		0, //level
		GL_RGBA, //internal format
		width, //width
		height, //height
		0, //border
		GL_RGBA, //data format
		GL_UNSIGNED_BYTE, //data type
		data); //data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);
}

void GUI::GenerateDigitTexture(const std::string& fileName) {
	int width, height, bpp;
	stbi_uc* data = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

	glDeleteTextures(1, &digitID);
	glGenTextures(1, &digitID);
	BindTexture(digitID);
	glTexImage2D(GL_TEXTURE_2D,
		0, //level
		GL_RGBA, //internal format
		width, //width
		height, //height
		0, //border
		GL_RGBA, //data format
		GL_UNSIGNED_BYTE, //data type
		data); //data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);
}

void GUI::GenerateCameraTexture(cv::Mat& cameraImage) {

	if (cameraImage.empty()) {
		std::cout << "image empty" << std::endl;
	}
	else {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glDeleteTextures(1, &cameraID);
		glGenTextures(1, &cameraID);
		BindTexture(cameraID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D,     // Type of texture
			0,							// Pyramid level (for mip-mapping) - 0 is the top level
			GL_RGB,						// Internal colour format to convert to
			cameraImage.cols,			// Image width  i.e. 640 for Kinect in standard mode
			cameraImage.rows,			// Image height i.e. 480 for Kinect in standard mode
			0,							// Border width in pixels (can either be 1 or 0)
			GL_RGB,						// Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			GL_UNSIGNED_BYTE,			// Image data type
			cameraImage.ptr());			// The actual image data itself
	}
}

void GUI::BindTexture(GLuint id) {
	glBindTexture(GL_TEXTURE_2D, id);
}

void GUI::showPowerupIcon() {
	BindTexture(powerupID);
	tigl::drawVertices(GL_QUADS, powerupVerts);
}