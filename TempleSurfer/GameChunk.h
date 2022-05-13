#pragma once

#include "GameObject.h"
#include "list"
#include <glm/glm.hpp>
#include <memory>

class GameChunk {
private:
	std::list<std::shared_ptr<GameObject>> gameObjects;
	glm::vec3 gamePosition;
public:
	GameChunk(std::list<std::shared_ptr<GameObject>> parameterObjects, glm::vec3 position);
	~GameChunk();

	// Chunk methods that define the chunk
	void moveChunk(glm::vec3 position);
	void addObject(std::shared_ptr<GameObject> object);

	// Standart draw and update methods
	void draw();
	void update(float elapsed_time);
};