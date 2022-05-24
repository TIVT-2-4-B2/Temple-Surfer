#include "Presets.h"
#include "FloorComponent.h"
#include "TextureComponent.h"
#include "PaneComponent.h"

void AddFloor(std::list<std::shared_ptr<GameObject>>& gameObjects)
{
	std::shared_ptr<GameObject> floor = std::make_shared<GameObject>();
	floor->position = glm::vec3(0, 0, 0);
	floor->addComponent(std::make_shared<FloorComponent>());
	floor->addComponent(std::make_shared<TextureComponent>("models/floor/sand2.jpg"));
	gameObjects.push_back(floor);

	//Adding sides
	std::shared_ptr<GameObject> leftSide = std::make_shared<GameObject>();
	leftSide->position = glm::vec3(0, 0, 0);
	leftSide->addComponent(std::make_shared<PaneComponent>(
		glm::vec3(FLOOR_WIDTH, 0, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(FLOOR_WIDTH + 8, -4, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(FLOOR_WIDTH + 8, -4, 0),
		glm::vec3(FLOOR_WIDTH, 0, 0),
		glm::vec4(0, 0, 0, 1)));
	leftSide->addComponent(std::make_shared<TextureComponent>("models/cube/tex2.bmp"));
	gameObjects.push_back(leftSide);

	std::shared_ptr<GameObject> rightSide = std::make_shared<GameObject>();
	rightSide->position = glm::vec3(0, 0, 0);
	rightSide->addComponent(std::make_shared<PaneComponent>(
		glm::vec3(-FLOOR_WIDTH, 0, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(-FLOOR_WIDTH - 8, -4, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(-FLOOR_WIDTH - 8, -4, 0),
		glm::vec3(-FLOOR_WIDTH, 0, 0),
		glm::vec4(0, 0, 0, 1)));
	rightSide->addComponent(std::make_shared<TextureComponent>("models/cube/tex2.bmp"));
	gameObjects.push_back(rightSide);
}
