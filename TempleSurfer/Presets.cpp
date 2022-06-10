#define _USE_MATH_DEFINES
#include <math.h>

#include "Presets.h"
#include "FloorComponent.h"
#include "TextureComponent.h"
#include "PaneComponent.h"
#include "MoveToComponent.h"
#include "OBJComponent.h"
#include "CubeComponent.h"
#include "CollisionComponent.h"
#include "PowerUpComponent.h"

void AddFloor(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	//Adding bottom floor
	std::shared_ptr<GameObject> floor = std::make_shared<GameObject>();
	floor->position = glm::vec3(0, 0, 0);
	floor->addComponent(std::make_shared<FloorComponent>());
	floor->addComponent(std::make_shared<TextureComponent>("models/floor/sand2.jpg"));
	gameObjects.push_back(floor);

	const int xOffset = 8;
	const int yCoordinate = 3;
	const int yWater = 2;

	//Adding sides
	std::shared_ptr<GameObject> leftSide = std::make_shared<GameObject>();
	leftSide->position = glm::vec3(0, 0, 0);
	leftSide->addComponent(std::make_shared<PaneComponent>(
		glm::vec3(FLOOR_WIDTH, -1, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(FLOOR_WIDTH + xOffset, yCoordinate, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(FLOOR_WIDTH + xOffset, yCoordinate, 0),
		glm::vec3(FLOOR_WIDTH, -1, 0),
		glm::vec4(0, 0, 0, 1)));
	leftSide->addComponent(std::make_shared<TextureComponent>("models/floor/sand2.jpg"));
	gameObjects.push_back(leftSide);

	std::shared_ptr<GameObject> rightSide = std::make_shared<GameObject>();
	rightSide->position = glm::vec3(0, 0, 0);
	rightSide->addComponent(std::make_shared<PaneComponent>(
		glm::vec3(-FLOOR_WIDTH, -1, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(-FLOOR_WIDTH - xOffset, yCoordinate, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(-FLOOR_WIDTH - xOffset, yCoordinate, 0),
		glm::vec3(-FLOOR_WIDTH, -1, 0),
		glm::vec4(0, 0, 0, 1)));
	rightSide->addComponent(std::make_shared<TextureComponent>("models/floor/sand2.jpg"));
	gameObjects.push_back(rightSide);
	//Adding water
	std::shared_ptr<GameObject> waterPane = std::make_shared<GameObject>();
	waterPane->position = glm::vec3(0, 0, 0);
	waterPane->addComponent(std::make_shared<PaneComponent>(
		glm::vec3(-FLOOR_WIDTH - xOffset, yWater, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(FLOOR_WIDTH + xOffset, yWater, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(FLOOR_WIDTH + xOffset, yWater, 0),
		glm::vec3(-FLOOR_WIDTH - xOffset, yWater, 0),
		glm::vec4(0, 0.4f, 0.6f, 0.5f)));
	gameObjects.push_back(waterPane);

	//Adding side floors
	//Right side
	std::shared_ptr<GameObject> floorPaneRight = std::make_shared<GameObject>();
	floorPaneRight->position = glm::vec3(0, 0, 0);
	floorPaneRight->addComponent(std::make_shared<PaneComponent>(
		glm::vec3(FLOOR_WIDTH + xOffset, yCoordinate, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(FLOOR_WIDTH + xOffset * 3, yCoordinate, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(FLOOR_WIDTH + xOffset * 3, yCoordinate, 0),
		glm::vec3(FLOOR_WIDTH + xOffset, yCoordinate, 0),
		glm::vec4(0, 0, 0, 1)));
	floorPaneRight->addComponent(std::make_shared<TextureComponent>("models/floor/sand2.jpg"));
	gameObjects.push_back(floorPaneRight);

	//Left side
	std::shared_ptr<GameObject> floorPaneLeft = std::make_shared<GameObject>();
	floorPaneLeft->position = glm::vec3(0, 0, 0);
	floorPaneLeft->addComponent(std::make_shared<PaneComponent>(
		glm::vec3(-FLOOR_WIDTH - xOffset, yCoordinate, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(-FLOOR_WIDTH - xOffset * 3, yCoordinate, FLOOR_LENGTH + FLOOR_OVERLAP),
		glm::vec3(-FLOOR_WIDTH - xOffset * 3, yCoordinate, 0),
		glm::vec3(-FLOOR_WIDTH - xOffset, yCoordinate, 0),
		glm::vec4(0, 0, 0, 1)));
	floorPaneLeft->addComponent(std::make_shared<TextureComponent>("models/floor/sand2.jpg"));
	gameObjects.push_back(floorPaneLeft);

	// Spawn objects on the sides
	// List of objects
	PresetList objectList;
	PushMultiplePresets(objectList, AddCactusGroup, 3);
	PushMultiplePresets(objectList, AddEmpty, 2);
	PushMultiplePresets(objectList, AddCamel, 1);

	float increment = FLOOR_LENGTH / 6;
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(FLOOR_WIDTH + xOffset + (rand() % 5), yCoordinate, increment * 1), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(FLOOR_WIDTH + xOffset + (rand() % 5), yCoordinate, increment * 3), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(FLOOR_WIDTH + xOffset + (rand() % 5), yCoordinate, increment * 5), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(-(FLOOR_WIDTH + xOffset + (rand() % 5)), yCoordinate, increment * 1), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(-(FLOOR_WIDTH + xOffset + (rand() % 5)), yCoordinate, increment * 3), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(-(FLOOR_WIDTH + xOffset + (rand() % 5)), yCoordinate, increment * 5), glm::vec3(1), glm::vec4(1));
}

void PushMultiplePresets(PresetList& presets, PresetFunc func, int count)
{
	for (size_t i = 0; i < count; i++)
	{
		presets.emplace_back(func);
	}
}

void AddEmpty(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{

}

void AddCactusGroup(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	AddCactus(gameObjects, pos, glm::vec3(0.01f));
	AddCactus(gameObjects, pos + glm::vec3(-0.5f, 0, 0), glm::vec3(0.005f));
	AddCactus(gameObjects, pos + glm::vec3(0.5f, 0, 0), glm::vec3(0.005f));
	AddCactus(gameObjects, pos + glm::vec3(0, 0, -0.5f), glm::vec3(0.005f));
	AddCactus(gameObjects, pos + glm::vec3(0, 0, 0.5f), glm::vec3(0.005f));
}

void AddCactus(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	std::shared_ptr<GameObject> cactus = std::make_shared<GameObject>();
	cactus->position = pos;

	//Add components to cactus object
	cactus->addComponent(std::make_shared<MoveToComponent>(pos));
	cactus->addComponent(std::make_shared<OBJComponent>("models/cactus/10436_Cactus_v1_max2010_it2.obj"));
	cactus->addComponent(std::make_shared<CollisionComponent>(glm::vec3(1, 1, 1)));
	cactus->scale = size;
	cactus->rotation = glm::vec3(-(M_PI / 2.0f), 0, 0);
	gameObjects.push_back(cactus);
}

void AddCamel(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	std::shared_ptr<GameObject> camel = std::make_shared<GameObject>();
	camel->position = pos;

	//Add components to object
	camel->addComponent(std::make_shared<MoveToComponent>(pos));
	camel->addComponent(std::make_shared<OBJComponent>("models/camel/Camel.obj"));
	camel->addComponent(std::make_shared<CollisionComponent>(glm::vec3(1, 1, 1)));
	camel->scale = glm::vec3(0.005f);
	const int rotationCount = 8;
	camel->rotation = glm::vec3(-(M_PI / 2.0f), 0, (rand() % rotationCount) * (2.0f / (float)rotationCount) * M_PI);
	gameObjects.push_back(camel);
}

void AddTugboat(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	std::shared_ptr<GameObject> boat = std::make_shared<GameObject>();
	glm::vec3 boatPos = pos;
	boat->position = boatPos;

	//Add components to boat object
	boat->addComponent(std::make_shared<MoveToComponent>(boatPos));
	boat->addComponent(std::make_shared<OBJComponent>("models/tugboat/12218_tugboat_v1_L2.obj"));
	boat->addComponent(std::make_shared<CollisionComponent>(glm::vec3(1, 1, 1)));
	boat->scale = glm::vec3(0.001f, 0.001f, 0.001f);
	std::vector<float> leftRight = { 0.0f, M_PI };
	boat->rotation = glm::vec3(-(M_PI / 2.0f), 0, leftRight.at(rand() % leftRight.size()));
	gameObjects.push_back(boat);
}

void AddBoat(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	std::shared_ptr<GameObject> boat = std::make_shared<GameObject>();
	glm::vec3 boatPos = pos;
	boat->position = boatPos;

	//Add components to boat object
	boat->addComponent(std::make_shared<MoveToComponent>(boatPos));
	boat->addComponent(std::make_shared<OBJComponent>("models/boat/12219_boat_v2_L2.obj"));
	boat->addComponent(std::make_shared<CollisionComponent>(glm::vec3(1, 1, 1)));
	boat->scale = glm::vec3(0.002f);
	std::vector<float> leftRight = { 0.0f, (float)M_PI };
	boat->rotation = glm::vec3(-1.57079633f, 0, leftRight.at(rand() % leftRight.size()));
	gameObjects.push_back(boat);
}

void AddContainer(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	std::shared_ptr<GameObject> container = std::make_shared<GameObject>();
	glm::vec3 contianerPos = pos + glm::vec3(1, -0.5f, 0);
	container->position = contianerPos;

	//Add components to container object
	container->addComponent(std::make_shared<MoveToComponent>(contianerPos));
	container->addComponent(std::make_shared<OBJComponent>("models/container_v2/12281_Container_v2_L2.obj"));
	container->addComponent(std::make_shared<CollisionComponent>(glm::vec3(0.007f)));
	container->scale = glm::vec3(0.007f);
	std::vector<float> leftRight = { 0.0f, (float)M_PI };
	container->rotation = glm::vec3(-1.57079633f, -1.57079633f, leftRight.at(rand() % leftRight.size()));
	gameObjects.push_back(container);
}

void AddCube(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	std::shared_ptr<GameObject> cube = std::make_shared<GameObject>();
	cube->position = pos;
	cube->addComponent(std::make_shared<CubeComponent>(size, color));
	cube->addComponent(std::make_shared<CollisionComponent>(size));
	gameObjects.push_back(cube);
}

void AddPowerUp(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
{
	std::shared_ptr<GameObject> powerUp = std::make_shared<GameObject>();
	powerUp->position = pos;
	powerUp->addComponent(std::make_shared<CubeComponent>(size, color)); //To Do: Change this to a proper powerup model.
	powerUp->addComponent(std::make_shared<PowerUpComponent>(size));
	gameObjects.push_back(powerUp);
}
