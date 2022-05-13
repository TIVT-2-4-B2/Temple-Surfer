#include "GameChunk.h"
#include "glm/glm.hpp"


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
	glm::vec3 diffPos = gamePosition - position;
	gamePosition = position;
	
	for (auto &object : gameObjects) {
		object->position += diffPos;
	}
}

void GameChunk::addObject(std::shared_ptr<GameObject> object)
{
	gameObjects.push_back(object);
}

void GameChunk::draw()
{
	for (auto& object : gameObjects) {
		object->draw();
	}
}

void GameChunk::update(float elapsedTime)
{
	for (auto& object : gameObjects) {
		object->update(elapsedTime);
	}
}

