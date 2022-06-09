#include "Presets.h"
#include "FloorComponent.h"
#include "TextureComponent.h"
#include "PaneComponent.h"
#include "MoveToComponent.h"
#include "OBJComponent.h"
#include "CubeComponent.h"
#include "CollisionComponent.h"

void AddFloor(std::list<std::shared_ptr<GameObject>>& gameObjects, glm::vec3 pos, glm::vec3 size, glm::vec4 color)
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

	// Spawn objects on the sides
	// List of objects
	PresetList objectList;
	objectList.emplace_back(AddCactusGroup);
	objectList.emplace_back(AddCactusGroup);
	objectList.emplace_back(AddCactusGroup);
	objectList.emplace_back(AddEmpty);
	objectList.emplace_back(AddEmpty);
	objectList.emplace_back(AddCamel);
	
	float increment = FLOOR_LENGTH / 6;
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(FLOOR_WIDTH + 8 + (rand() % 5), 2, increment * 1), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(FLOOR_WIDTH + 8 + (rand() % 5), 2, increment * 3), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(FLOOR_WIDTH + 8 + (rand() % 5), 2, increment * 5), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(-(FLOOR_WIDTH + 8 + (rand() % 5)), 2, increment * 1), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(-(FLOOR_WIDTH + 8 + (rand() % 5)), 2, increment * 3), glm::vec3(1), glm::vec4(1));
	objectList.at(rand() % objectList.size())(gameObjects, glm::vec3(-(FLOOR_WIDTH + 8 + (rand() % 5)), 2, increment * 5), glm::vec3(1), glm::vec4(1));
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
	cactus->rotation = glm::vec3(-1.57079633f, 0, 0);
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
	camel->rotation = glm::vec3(-1.57079633f, 0, 0);
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
	std::vector<float> leftRight = { 0.0f, 3.14f };
	boat->rotation = glm::vec3(-1.57079633f, 0, leftRight.at(rand() % leftRight.size()));
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
	std::vector<float> leftRight = { 0.0f, 3.14f };
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
	std::vector<float> leftRight = { 0.0f, 3.14f };
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
