#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>

#include "OBJComponent.h"
#include <memory>
#include "AudioManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ChunkGenerator.h"
#include "FloorComponent.h"
#include "GameChunk.h"
#include "GameScene.h"
#include "Presets.h"
#include "MoveToComponent.h"
#include "CollisionComponent.h"
#include "GUI.h"

#include "Vision.h"

#include <iostream>

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

bool isPlaying = false;

double lastFrameTime = 0;

std::shared_ptr<GameObject> player;
std::shared_ptr<GameChunk> chunk;
std::list<std::shared_ptr<GameObject>> list;
std::shared_ptr<GameScene> scene;
std::shared_ptr<Vision> vision;
std::shared_ptr<GUI> gui;
ChunkGenerator generator;

int WindowWidth;
int WindowHeight;

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
			gui->drawMenu();
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

	gui = std::make_shared<GUI>(WindowWidth, WindowHeight);

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

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
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

	gui->drawGUI(vision);
}