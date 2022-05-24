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

void PlayerComponent::update(float _)
{
	if (jumpOrCrouch)
	{
		std::chrono::system_clock::duration duration = std::chrono::system_clock::now() - lastTime;
		std::chrono::milliseconds mSeconds = std::chrono::duration_cast<std::chrono::milliseconds> (duration);
		if(mSeconds.count() > 200)
		{
			resetY();
			jumpOrCrouch = false;
		}
	}
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
	jumpOrCrouch = true;
	lastTime = std::chrono::system_clock::now();
}

void PlayerComponent::crouch()
{
	gameObject->getComponent<MoveToComponent>()->target.y = 0;
	jumpOrCrouch = true;
	lastTime = std::chrono::system_clock::now();
}

void PlayerComponent::resetY()
{
	gameObject->getComponent<MoveToComponent>()->target.y = 1;
}
