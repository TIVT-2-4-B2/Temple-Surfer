#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
using tigl::Vertex;
#include "OBJComponent.h"
#include <memory>

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


#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

void init();
void update();
void start();
void createScene();
void draw();
void drawMenu();

bool isPlaying = false;

double lastFrameTime = 0;

std::shared_ptr<GameObject> player;
std::shared_ptr<GameChunk> chunk;
std::list<std::shared_ptr<GameObject>> list;
std::shared_ptr<GameScene> scene;
ChunkGenerator generator;

int main(void)
{
	if (!glfwInit()) {
		throw "Could not initialize glwf";
	}	

	int count;

	GLFWmonitor** monitors = glfwGetMonitors(&count);

	const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);

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

	tigl::init();

	init();

	Vision vision;

	while (!glfwWindowShouldClose(window))
	{

		//Check if a key is pressed
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !isPlaying)
		{
			start();
			isPlaying = true;
			lastFrameTime = glfwGetTime();
			vision = Vision(player->getComponent<PlayerComponent>());
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
			vision.visionUpdate();
		}

	}

	glfwTerminate();

	return 0;
}

//Initiate the game
void init()
{
	glEnable(GL_DEPTH_TEST);
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->setLightPosition(0, (glm::vec3(0, 7, 12)));
	tigl::shader->setLightAmbient(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightSpecular(0, glm::vec3(1,1,1));
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
	player->addComponent(std::make_shared<OBJComponent>("models/car/honda_jazz.obj"));
#endif
	player->addComponent(std::make_shared<CollisionComponent>(glm::vec3(1, 1, 1))); //ToDo change to accurate hitbox.
	player->addComponent(std::make_shared<PlayerComponent>());
	player->scale = glm::vec3(0.03f, 0.03f, 0.03f);
	player->rotation = glm::vec3(0, -1.57079633f, 0);
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

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 1000.0f);

	//Set camera
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);
	tigl::shader->enableFog(false);

	//Draw orange quad for menu
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PCN(glm::vec3(-100, 0, -100), glm::vec4(1, 0.4, 0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(-100, 0, 100), glm::vec4(1, 0.4, 0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(100, 0, 100), glm::vec4(1, 0.4, 0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCN(glm::vec3(100, 0, -100), glm::vec4(1, 0.4, 0, 1), glm::vec3(0, 1, 0)));
	tigl::end();
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
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(player->position.x,7,12), glm::vec3(player->position.x,0,0), glm::vec3(0,1,0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	scene->draw();
}
