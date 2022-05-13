#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
using tigl::Vertex;


#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "GameChunk.h"
#include "GameScene.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
#include "FloorComponent.h"
#include "MoveToComponent.h"
#include "SpinComponent.h"
#include "TimerJumper.h"
#include "EnemyComponent.h"
#include <iostream>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

void init();
void update();
void draw();

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}


double lastFrameTime = 0;
GameObject* movingObject;

std::shared_ptr<GameObject> player;
std::shared_ptr<GameChunk> chunk;
std::shared_ptr<GameScene> scene;

void init()
{
	glEnable(GL_DEPTH_TEST);

	scene = std::make_shared<GameScene>();

	player = std::make_shared<GameObject>();
	player->position = glm::vec3(0, 1, 5);

	float r = rand() / static_cast<float>(RAND_MAX);
	float g = rand() / static_cast<float>(RAND_MAX);
	float b = rand() / static_cast<float>(RAND_MAX);

	player->addComponent(new CubeComponent(glm::vec3(1,1,5), glm::vec4(r,g,b, 1)));
	player->addComponent(new PlayerComponent());
	scene->addGameObject(player);

	std::list<std::shared_ptr<GameObject>> list;
	for (int i = 0; i < 100; i++)
	{
		std::shared_ptr<GameObject> o = std::make_shared<GameObject>();
		o->position = glm::vec3(rand() % 30 - 15, 1, rand() % 30 - 15);
		float r = rand() / static_cast<float>(RAND_MAX);
		float g = rand() / static_cast<float>(RAND_MAX);
		float b = rand() / static_cast<float>(RAND_MAX);
		o->addComponent(new CubeComponent(glm::vec3(1,1,1), glm::vec4(r, g, b, 1)));
		o->addComponent(new MoveToComponent());
		o->getComponent<MoveToComponent>()->target = o->position;
		o->addComponent(new EnemyComponent());
		list.push_back(o);
	}
	chunk = std::make_shared<GameChunk>(list, glm::vec3(0, 0, 0));

	scene->addGameChunk(chunk);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
}

void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	scene->update(deltaTime);
}




void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 1000.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);
	//temporary draw floor
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PC(glm::vec3(-50, 0, -50), glm::vec4(1, 0, 0, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(-50, 0, 50), glm::vec4(0, 1, 0, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(50, 0, 50), glm::vec4(0, 0, 1, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(50, 0, -50), glm::vec4(0, 0, 1, 1)));
	tigl::end();

	scene->draw();
}




