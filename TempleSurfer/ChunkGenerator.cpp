#include "ChunkGenerator.h"

#include "FloorComponent.h"
#include "CubeComponent.h"
#include <chrono>

using enum ChunkObstacle;

void ChunkGenerator::generatorInit()
{
	// Seeds the random function
	srand(std::chrono::system_clock::now().time_since_epoch().count());

	// Loading all the possible presets
	// TODO make with file IO. Temp solution right here..
	presets.push_back({ {{{NONE},{JUMP},{BLOCK}},
				{{BLOCK},{BLOCK},{NONE}},
				{{NONE},{NONE},{DUCK}}}});
	presets.push_back({ {{{NONE},{NONE},{BLOCK}},
			{{BLOCK},{NONE},{NONE}},
			{{NONE},{NONE},{BLOCK}}} });
	presets.push_back({ {{{NONE},{JUMP},{BLOCK}},
			{{NONE},{BLOCK},{BLOCK}},
			{{NONE},{NONE},{BLOCK}}} });
	presets.push_back({ {{{NONE},{JUMP},{BLOCK}},
			{{BLOCK},{BLOCK},{NONE}},
			{{JUMP},{NONE},{JUMP}}} });
}

std::shared_ptr<GameChunk> ChunkGenerator::getChunk()
{
	// Returning a random chunk that is generated.
	return buildChunk(presets.at(rand() % presets.size()));
}

std::shared_ptr<GameChunk> ChunkGenerator::buildChunk(ChunkPreset preset)
{
	// Generating objects
	std::list<std::shared_ptr<GameObject>> gameObjects;

	// Adding in the floor
	std::shared_ptr<GameObject> floor = std::make_shared<GameObject>();
	floor->position = glm::vec3(0, 0, 0);
	floor->addComponent(std::make_shared<FloorComponent>());
	gameObjects.push_back(floor);
	float xPos;
	float zPos;
	// Adding in the generated config.
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		zPos = ((1.0f / 6.0f) + (1.0f / 3.0f) * i) * FLOOR_LENGTH;
		for (int j = 0; j < MATRIX_SIZE; j++) {
			auto cube = std::make_shared<GameObject>();
			xPos = -((2.0f / 3.0f) * FLOOR_WIDTH) + ((2.0f / 3.0f) * FLOOR_WIDTH) * j;  // X
			switch (preset.obstacles[i][j]) {
				case BLOCK:
					cube->position = glm::vec3(xPos, 2, zPos);
					cube->addComponent(std::make_shared<CubeComponent>(glm::vec3(1, 2, 1), glm::vec4(0, 1.0f, 1.0f, 1)));
					break;
				case JUMP:
					cube->position = glm::vec3(xPos, 1, zPos);
					cube->addComponent(std::make_shared<CubeComponent>(glm::vec3(1, 1, 1), glm::vec4(1.0f, 1.0f, 0,  1)));
					break;
				case DUCK:
					cube->position = glm::vec3(xPos, 3, zPos);
					cube->addComponent(std::make_shared<CubeComponent>(glm::vec3(1, 1, 1), glm::vec4(1.0f,0, 1.0f, 1)));
					break;
				case NONE:
					continue;
			}
			gameObjects.push_back(cube);
		}
	}

	// Filling the chunk
	std::shared_ptr<GameChunk> chunkPointer = std::make_shared<GameChunk>(gameObjects, glm::vec3(0, 0, Z_THRESHOLD - (CHUNKS_ON_SCREEN * FLOOR_LENGTH)));
	return chunkPointer;
}
