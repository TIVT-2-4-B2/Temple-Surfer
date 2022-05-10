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

void GameObject::addComponent(Component * component)
{
	component->setGameObject(this);
	components.push_back(component);

	if (!drawComponent)
		drawComponent = dynamic_cast<DrawComponent*>(component);

	auto bb = dynamic_cast<BoundingBoxComponent*>(component);
	if (bb)
		boundingBox.push_back(bb);
}

std::list<Component*> GameObject::getComponents()
{
	return components;
}


void GameObject::draw(const glm::mat4 &parentMatrix)
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
	drawComponent->draw();
}

bool GameObject::collides(GameObject* other)
{
	if (boundingBox.empty())
		return false;
	for (auto bb : boundingBox)
		if (bb->collides(other))
			return true;

	return false;
}

void GameObject::update(float elapsedTime)
{
	for (auto &c : components)
		c->update(elapsedTime);
}