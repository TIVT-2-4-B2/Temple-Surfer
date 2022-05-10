#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
using tigl::Vertex;

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
int move = 0;

void init()
{
	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
			if (key == GLFW_KEY_LEFT)
				move--;
			if (key == GLFW_KEY_RIGHT)
				move++;
		});
}

void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tigl::shader->enableColor(true);

	tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(70.0f), 1280 / (float)720, 0.1f, 200.0f));
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(move, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 0, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 0, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0, 0, 0)));

	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec4(0, 1.0f, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec4(0, 1.0f, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec4(0, 1.0f, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec4(0, 1.0f, 0, 0)));

	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(0, 0, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(0, 0, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec4(0, 0, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec4(0, 0, 1.0f, 0)));

	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 0, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 0, 0)));

	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 0, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec4(1.0f, 0, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec4(1.0f, 0, 1.0f, 0)));

	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec4(0, 1.0f, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(0, 1.0f, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec4(0, 1.0f, 1.0f, 0)));
	tigl::addVertex(Vertex::PC(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec4(0, 1.0f, 1.0f, 0)));
	tigl::end();
}



