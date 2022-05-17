#include "PlayerComponent.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>

extern GLFWwindow* window;


PlayerComponent::PlayerComponent()
{
}

PlayerComponent::~PlayerComponent()
{
}

void PlayerComponent::update(float _)
{
	const float offset = (10.0f * (2.0f / 3.0f));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gameObject->position.x = 0;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gameObject->position.x = -offset;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gameObject->position.x = offset;
}
