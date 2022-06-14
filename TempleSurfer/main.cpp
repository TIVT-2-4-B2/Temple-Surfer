#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>

#include "OBJComponent.h"
#include <memory>
#include "AudioManager.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stack>
#include <algorithm>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ChunkGenerator.h"
#include "FloorComponent.h"
#include "GameChunk.h"
#include "GameScene.h"
#include "GameObject.h"
#include "Presets.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
#include "TextureComponent.h"
#include "MoveToComponent.h"
#include "SpinComponent.h"
#include "TimerJumper.h"
#include "EnemyComponent.h"
#include "CollisionComponent.h"

#include "Vision.h"

#include <iostream>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using tigl::Vertex;
using namespace cv;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "bass.lib")

GLFWwindow* window;

void init();
void update();
void start();
void createScene();
void draw();
void drawMenu();
void drawGUI();
void GenerateTexture(cv::Mat& image);
std::vector<int> intToDigits(int number);
void showScore(std::vector<int> digits);
void BindTexture();

void GenerateImageTexture(const std::string& fileName);
void BindImageTexture();

bool isPlaying = false;

double lastFrameTime = 0;

const int textureInterval = 1;
const int digitWidth = 60;
const int digitHeight = 80;
const int digitOffsetX = 20;
const int digitOffsetY = 20;

std::shared_ptr<GameObject> player;
std::shared_ptr<GameChunk> chunk;
std::list<std::shared_ptr<GameObject>> list;
std::shared_ptr<GameScene> scene;
std::shared_ptr<Vision> vision;
ChunkGenerator generator;

int WindowWidth;
int WindowHeight;

GLuint textureID;
int textureCount = 0;

int main(void)
{
	if (!glfwInit()) {
		throw "Could not initialize glwf";
	}

	int count;

	GLFWmonitor** monitors = glfwGetMonitors(&count);

	const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);
	WindowWidth = mode->width;
	WindowHeight = mode->height;


	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//Place null at monitors[0] 
	window = glfwCreateWindow(mode->width, mode->height, "Temple surfer", NULL, NULL);
	//window = glfwCreateWindow(mode->width, mode->height, "Temple surfer", monitors[0], NULL);

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	AudioManager::instance()->initAudio();
	AudioManager::instance()->loopMusic("Resources/backgroundmusic.mp3");

	tigl::init();

	

	init();


	while (!glfwWindowShouldClose(window))
	{

		//Check if a key is pressed
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !isPlaying)
		{
			start();
			isPlaying = true;
			lastFrameTime = glfwGetTime();
			vision = std::make_shared<Vision>(player->getComponent<PlayerComponent>());
		}
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			isPlaying = false;
		}

		//If the game hasn't been started, draw the menu and not the scene
		if (!isPlaying)
		{
			drawMenu();
			glfwSwapBuffers(window);
			glfwPollEvents();
			continue;
		}

		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (isPlaying)
		{
			vision->visionUpdate();
		}

	}

	glfwTerminate();

	return 0;
}

//Initiate the game
void init()
{
	// Precache objs
	std::string objs[5] = { 
		"models/evergiven/EverGiven.obj",
		"models/tugboat/12218_tugboat_v1_L2.obj",
		"models/boat/12219_boat_v2_L2.obj",
		"models/container_v2/12281_Container_v2_L2.obj",
		"models/fish/12265_Fish_v1_L2.obj"
	};
	for (int i = 0; i < 4; i++) {
		std::shared_ptr<OBJComponent> obj1 = std::make_shared<OBJComponent>(objs[i]);
	}
	std::cout << "Loaded precache" << std::endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->setLightPosition(0, (glm::vec3(0, 7, 12)));
	tigl::shader->setLightAmbient(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightSpecular(0, glm::vec3(1, 1, 1));
	tigl::shader->setFogColor(glm::vec3(0.3f, 0.4f, 0.6f));
	tigl::shader->setFogExp2(0.04f);
	tigl::shader->setShinyness(0);

	createScene();

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
		});

	update();

}

//#define FPS_DEBUG
//Update everything in the scene
void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

#ifdef FPS_DEBUG
	std::cout << 1 / deltaTime << " FPS" << std::endl;
#endif // FPS_DEBUG

	scene->update(deltaTime);
}

//Start the game (Also supports restarting)
void start() {
	scene = nullptr;
	player = nullptr;
	chunk = nullptr;
	list.clear();

	createScene();
}

//Create a scene
void createScene() {
	generator = ChunkGenerator();
	generator.generatorInit();

	scene = std::make_shared<GameScene>();

	//Create player object
	player = std::make_shared<GameObject>();
	glm::vec3 playerPos = glm::vec3(0, 1, 3);
	player->position = playerPos;

	//Add components to player object
	player->addComponent(std::make_shared<MoveToComponent>(playerPos));
#ifndef COLLISION_DEBUG
	player->addComponent(std::make_shared<OBJComponent>("models/dolphin/", 0.05f));
#endif
	player->addComponent(std::make_shared<CollisionComponent>(glm::vec3(0.75f, 1, 1.2f))); //ToDo change to accurate hitbox.
	player->addComponent(std::make_shared<PlayerComponent>());
	player->scale = glm::vec3(0.7f, 0.7f, 0.7f);
	player->rotation = glm::vec3(0, -1 * (float)M_PI, 0);
	scene->addGameObject(player);

	std::list<std::shared_ptr<GameObject>> objectList;

	//Create floor object
	AddFloor(objectList);

	std::shared_ptr<GameChunk> chunk = std::make_shared<GameChunk>(objectList, glm::vec3(0, 0, Z_THRESHOLD - FLOOR_LENGTH));
	scene->addGameChunk(chunk);
	for (size_t i = 1; i < CHUNKS_ON_SCREEN; i++)
	{
		auto nextChunk = generator.getChunk();
		nextChunk->gamePosition = glm::vec3(0, 0, Z_THRESHOLD - (FLOOR_LENGTH * (i + 1)));
		scene->addGameChunk(nextChunk);
	}
}

//Draw the main menu
void drawMenu()
{
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
	GenerateImageTexture(fileName);
	BindImageTexture();

	glDisable(GL_DEPTH_TEST);

	//Draw orange quad for menu
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PCTN(glm::vec3(0, WindowHeight, 0), glm::vec4(0, 0, 0, 1), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(WindowWidth, WindowHeight, 0), glm::vec4(0, 0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(WindowWidth, 0, 0), glm::vec4(0, 0, 0, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(0, 0, 0), glm::vec4(0, 0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	tigl::end();

	glEnable(GL_DEPTH_TEST);

	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);

}

//Draw the game scene
void draw()
{
	tigl::shader->enableFog(true);
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 1000.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(player->position.x, 7, 12), glm::vec3(player->position.x, 0, 0), glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	scene->draw();

	drawGUI();

}

void drawGUI() {

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
	textureCount = 1;
	if (textureCount % textureInterval == 0)
	{
		GenerateTexture(img);
	}
	else {
		BindTexture();
	}
	textureCount++;

	glDisable(GL_DEPTH_TEST);

	//Draw camera corner
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PCTN(glm::vec3(0, img.rows / 2, 0), glm::vec4(1, 0.4, 0, 1), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(img.cols / 2, img.rows / 2, 0), glm::vec4(1, 0.4, 0, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(img.cols / 2, 0, 0), glm::vec4(1, 0.4, 0, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(0, 0, 0), glm::vec4(1, 0.4, 0, 1), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	tigl::end();

	std::vector digits = intToDigits(score);
	showScore(digits);

	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);

	glEnable(GL_DEPTH_TEST);

}

void showScore(std::vector<int> digits) {

	for (int i = (digits.size() - 1); i >= 0; i--)
	{
		std::string fileName;
		fileName.append("Resources\\digit");
		fileName.append(std::to_string(digits.at(i)));
		fileName.append(".png");

		GenerateImageTexture(fileName);
		BindImageTexture();

		tigl::begin(GL_QUADS);
		tigl::addVertex(Vertex::PCTN(glm::vec3((WindowWidth - (i * digitWidth)) - digitWidth - digitOffsetX, digitHeight + digitOffsetY, 0), glm::vec4(0, 0, 0, 1), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
		tigl::addVertex(Vertex::PCTN(glm::vec3((WindowWidth - (i * digitWidth)) - digitOffsetX, digitHeight + digitOffsetY, 0), glm::vec4(0, 0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
		tigl::addVertex(Vertex::PCTN(glm::vec3((WindowWidth - (i * digitWidth)) - digitOffsetX, digitOffsetY, 0), glm::vec4(0, 0, 0, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
		tigl::addVertex(Vertex::PCTN(glm::vec3((WindowWidth - (i * digitWidth)) - digitWidth - digitOffsetX, digitOffsetY, 0), glm::vec4(0, 0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
		tigl::end();

	}

}

std::vector<int> intToDigits(int number) {
	std::vector<int> digits;

	while (number > 0) {
		digits.push_back(number % 10);
		number = number / 10;
	}

	return digits;
}

GLuint id;

void GenerateImageTexture(const std::string& fileName)
{
	int width, height, bpp;
	stbi_uc* data = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

	glDeleteTextures(1, &id);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
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

void BindImageTexture()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void GenerateTexture(cv::Mat& cameraImage)
{

	if (cameraImage.empty()) {
		std::cout << "image empty" << std::endl;
	}
	else {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glDeleteTextures(1, &textureID);
		glGenTextures(1, &textureID);
		BindTexture();

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

void BindTexture() {
	glBindTexture(GL_TEXTURE_2D, textureID);
}