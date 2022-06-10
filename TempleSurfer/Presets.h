#pragma once

#include <list>
#include <memory>
#include <vector>
#include <functional>
#include "GameObject.h"

using PresetFunc = std::function<void(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)>;
using PresetList = std::vector<PresetFunc>;

void PushMultiplePresets(PresetList& presets, PresetFunc func, int count);

void AddFloor(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos = glm::vec3(0), glm::vec3 size = glm::vec3(0), glm::vec4 color = glm::vec4(0));

void AddEmpty(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos = glm::vec3(0), glm::vec3 size = glm::vec3(0), glm::vec4 color = glm::vec4(0));

void AddCactusGroup(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size = glm::vec3(0), glm::vec4 color = glm::vec4(0));

void AddCactus(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size = glm::vec3(0.01f), glm::vec4 color = glm::vec4(0));

void AddCamel(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size = glm::vec3(0), glm::vec4 color = glm::vec4(0));

void AddEvergiven(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size = glm::vec3(0), glm::vec4 color = glm::vec4(0));

void AddTugboat(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size = glm::vec3(0), glm::vec4 color = glm::vec4(0));

void AddBoat(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size = glm::vec3(0), glm::vec4 color = glm::vec4(0));

void AddContainer(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size = glm::vec3(0), glm::vec4 color = glm::vec4(0));

void AddCube(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color);

void AddPowerUp(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color);


