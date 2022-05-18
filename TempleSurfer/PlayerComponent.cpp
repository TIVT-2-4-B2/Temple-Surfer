#include "PlayerComponent.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>
#include "MoveToComponent.h"

extern GLFWwindow* window;


PlayerComponent::PlayerComponent()
{
}

PlayerComponent::~PlayerComponent()
{
}

void PlayerComponent::update(float elapsedTime)
{	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveCenter();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveLeft();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveRight();
}

void PlayerComponent::moveLeft()
{
	gameObject->getComponent<MoveToComponent>()->target.x = -(10.0f * (2.0f / 3.0f));
}

void PlayerComponent::moveCenter()
{
	gameObject->getComponent<MoveToComponent>()->target.x = 0;
}

void PlayerComponent::moveRight()
{
	gameObject->getComponent<MoveToComponent>()->target.x = (10.0f * (2.0f / 3.0f));
}
