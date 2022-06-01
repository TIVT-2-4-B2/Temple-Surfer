#include "Presets.h"
#include "FloorComponent.h"
#include "TextureComponent.h"
#include "PaneComponent.h"
#include "MoveToComponent.h"
#include "OBJComponent.h"
#include "CubeComponent.h"

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

void AddTugboat(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos)
{
	std::shared_ptr<GameObject> boat = std::make_shared<GameObject>();
	glm::vec3 boatPos = pos;
	boat->position = boatPos;

	//Add components to player object
	boat->addComponent(std::make_shared<MoveToComponent>(boatPos));
	boat->addComponent(std::make_shared<OBJComponent>("models/tugboat/12218_tugboat_v1_L2.obj"));
	boat->scale = glm::vec3(0.001f, 0.001f, 0.001f);
	std::vector<float> leftRight = { 0.0f, 3.14f };
	boat->rotation = glm::vec3(-1.57079633f, 0, leftRight.at(rand() % leftRight.size()));
	gameObjects.push_back(boat);
}

void AddCube(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	std::shared_ptr<GameObject> cube = std::make_shared<GameObject>();
	cube->position = pos;
	cube->addComponent(std::make_shared<CubeComponent>(size, color));
	gameObjects.push_back(cube);
}
