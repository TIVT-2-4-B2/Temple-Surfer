#include "GameChunk.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>


GameChunk::GameChunk(std::list<std::shared_ptr<GameObject>> parameterObjects, glm::vec3 position)
{
	gameObjects = parameterObjects;
	gamePosition = position;
}

GameChunk::~GameChunk()
{
}

void GameChunk::moveChunk(glm::vec3 position)
{
	gamePosition = position;
}

void GameChunk::addObject(std::shared_ptr<GameObject> object)
{
	gameObjects.push_back(object);
}

void GameChunk::draw()
{
	// Calculating a new model matrix.
	const glm::mat4 & relativePos= glm::mat4(1.0f);
	glm::mat4 modelMatrix = relativePos;
	modelMatrix = glm::translate(modelMatrix, gamePosition);

	// Drawing objects.
	for (auto& object : gameObjects) {
		object->draw(modelMatrix);
	}
}

void GameChunk::update(float elapsedTime)
{
	// Updating componenents.
	for (auto& object : gameObjects) {
		object->update(elapsedTime);
	}
}

