#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
using tigl::Vertex;

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "GameObject.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
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


std::list<GameObject*> objects;
double lastFrameTime = 0;
GameObject* movingObject;

GameObject* player;

void init()
{
	glEnable(GL_DEPTH_TEST);

	/*for (int x = -10; x < 10; x += 2) {
		GameObject* o = new GameObject();
		o->position = glm::vec3(x, 0, 0);
		o->rotation.y = x*.25f;
		o->addComponent(new CubeComponent(1));
		o->addComponent(new SpinComponent(1));
		objects.push_back(o);
	}


	movingObject = new GameObject();
	movingObject->position = glm::vec3(0, 1, 3);
	movingObject->addComponent(new CubeComponent(1));
	movingObject->addComponent(new MoveToComponent());
	movingObject->getComponent<MoveToComponent>()->target = movingObject->position;
	movingObject->addComponent(new TimerJumper());
	objects.push_back(movingObject);
	*/

	player = new GameObject();
	player->position = glm::vec3(0, 1, 5);

	float r = rand() / static_cast<float>(RAND_MAX);
	float g = rand() / static_cast<float>(RAND_MAX);
	float b = rand() / static_cast<float>(RAND_MAX);

	player->addComponent(new CubeComponent(glm::vec3(1,1,5), glm::vec4(r,g,b, 1)));
	player->addComponent(new PlayerComponent());
	objects.push_back(player);

	for (int i = 0; i < 100; i++)
	{
		GameObject* o = new GameObject();
		o->position = glm::vec3(rand() % 30 - 15, 1, rand() % 30 - 15);
		float r = rand() / static_cast<float>(RAND_MAX);
		float g = rand() / static_cast<float>(RAND_MAX);
		float b = rand() / static_cast<float>(RAND_MAX);
		o->addComponent(new CubeComponent(glm::vec3(1,1,1), glm::vec4(r, g, b, 1)));
		o->addComponent(new MoveToComponent());
		o->getComponent<MoveToComponent>()->target = o->position;
		o->addComponent(new EnemyComponent());
		objects.push_back(o);
	}

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
}

std::vector<GameObject*> getColliders(GameObject* src)
{
	std::vector<GameObject*> ret;
	for (auto go : objects) {
		if (go != src && src->collides(go))
		{
			ret.push_back(go);
		}
	}
	return ret;
}

void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	for (auto& o : objects)
		o->update(deltaTime);


	std::vector<GameObject*> colliders = getColliders(player);

	if (player->collides(movingObject))
	{
		std::cout << "Collision!" << std::endl;
	}
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



	for (auto& o : objects)
		o->draw();
}




