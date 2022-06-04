#pragma once

#include <list>
#include <memory>
#include "GameObject.h"

void AddFloor(std::list<std::shared_ptr<GameObject>> &gameObjects);

void AddTugboat(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos);

void AddContainer(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos);

void AddCube(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color);

