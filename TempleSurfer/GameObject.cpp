#include "GameObject.h"
#include "DrawComponent.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "BoundingBoxComponent.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::addComponent(std::shared_ptr<Component> component)
{
	component->setGameObject(this);
	components.push_back(component);

	if (drawComponent == nullptr)
		drawComponent = dynamic_pointer_cast<DrawComponent>(component);

}

std::list<std::shared_ptr<Component>> GameObject::getComponents()
{
	return components;
}


void GameObject::draw(const glm::mat4& parentMatrix)
{
	if (!drawComponent)
		return;

	glm::mat4 modelMatrix = parentMatrix;
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);

	tigl::shader->setModelMatrix(modelMatrix);

	for (auto& c : components) {
		std::shared_ptr<DrawComponent> dc = dynamic_pointer_cast<DrawComponent>(c);
		if (dc != nullptr)
			dc->draw();
	}
}

void GameObject::update(float elapsedTime)
{
	for (auto& c : components)
		c->update(elapsedTime);
}

void GameObject::update(float elapsedTime, const glm::vec3& parentMatrix)
{
	for (auto& c : components) {
		std::shared_ptr<Component> dc = dynamic_pointer_cast<Component>(c);
		if (dc != nullptr)
			dc->update(elapsedTime, parentMatrix);
	}
}