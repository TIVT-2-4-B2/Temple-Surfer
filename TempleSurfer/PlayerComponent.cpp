#include "PlayerComponent.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>
#include "MoveToComponent.h"
#include <thread>
#include <chrono>

extern GLFWwindow* window;


PlayerComponent::PlayerComponent()
{
}

PlayerComponent::~PlayerComponent()
{
}

void PlayerComponent::update(float _)
{
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveCenter();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveLeft();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveRight();
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		jump();
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		crouch();
}

void PlayerComponent::moveLeft()
{
	gameObject->getComponent<MoveToComponent>()->target.x = -(FLOOR_WIDTH * (2.0f / 3.0f));
}

void PlayerComponent::moveCenter()
{
	gameObject->getComponent<MoveToComponent>()->target.x = 0;
}

void PlayerComponent::moveRight()
{
	gameObject->getComponent<MoveToComponent>()->target.x = (FLOOR_WIDTH * (2.0f / 3.0f));
}

void PlayerComponent::jump()
{
	gameObject->getComponent<MoveToComponent>()->target.y = 2;
	std::thread (resetY).detach();
}

void PlayerComponent::crouch()
{
	gameObject->getComponent<MoveToComponent>()->target.y = 0;
	std::thread (resetY).detach();
}

void PlayerComponent::resetY()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	gameObject->getComponent<MoveToComponent>()->target.y = 1;
}
