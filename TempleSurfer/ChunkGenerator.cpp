#include "ChunkGenerator.h"

#include "FloorComponent.h"
#include "CubeComponent.h"
#include "TextureComponent.h"
#include "CollisionComponent.h"
#include "Presets.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

using enum ChunkObstacle;

void ChunkGenerator::generatorInit()
{
	// Seeds the random function
	srand(std::chrono::system_clock::now().time_since_epoch().count());

	// Loading all the possible presets
	std::string line;
  	std::ifstream presetFile ("Resources/Presets.txt");
	
  	if (presetFile.is_open())
  	{
		ChunkPreset preset;
    	while ( getline (presetFile,line) )
    	{
			const int obstacleLines = 3;

			for (int i = 0; i < obstacleLines; i++)
			{
				const int offset = 2;
				int presetValue = stoi(line.substr(6 * i, 1));
				preset.obstacles[i][0] = getObstacleFromInt(presetValue);
				presetValue = stoi(line.substr(6 * i + offset, 1));
				preset.obstacles[i][1] = getObstacleFromInt(presetValue);
				presetValue = stoi(line.substr(6 * i + 2 * offset, 1));
				preset.obstacles[i][2] = getObstacleFromInt(presetValue);
			}
			presets.push_back(preset);
    	}
    	presetFile.close();
  	}
  	else
	{
		presets.push_back({ {{{NONE},{JUMP},{BLOCK}},
						{{BLOCK},{BLOCK},{NONE}},
						{{NONE},{NONE},{DUCK}}}});
		presets.push_back({ {{{NONE},{NONE},{BLOCK}},
						{{BLOCK},{NONE},{NONE}},
						{{NONE},{NONE},{BLOCK}}} });
		presets.push_back({ {{{NONE},{JUMP},{BLOCK}},
						{{NONE},{BLOCK},{BLOCK}},
						{{NONE},{NONE},{BLOCK}}} });
	}
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

	float xPos;
	float zPos;

	// List of block objects
	PresetList blockList;
	PushMultiplePresets(blockList, AddContainer, 1);

	// List of jump objects
	PresetList jumpList;
	PushMultiplePresets(jumpList, AddContainer, 3);
	PushMultiplePresets(jumpList, AddBoat, 1);

	// List of duck objects
	PresetList duckList;
	//duckList.emplace_back(AddCube);
	duckList.emplace_back(AddBoat);
	duckList.emplace_back(AddTugboat);

	// Adding in the generated config.
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		const float posOffset = 1.0f / 3.0f;
		zPos = (posOffset / 2.0f + posOffset * i) * FLOOR_LENGTH;
		for (int j = 0; j < MATRIX_SIZE; j++) {
			xPos = -(2.0f * posOffset * FLOOR_WIDTH) + (2.0f * posOffset * FLOOR_WIDTH) * j;  // X
			switch (preset.obstacles[i][j]) {
				case BLOCK:
					blockList.at(rand() % blockList.size())(gameObjects, glm::vec3(xPos, 2, zPos), glm::vec3(1, 2, 1), glm::vec4(0, 1.0f, 1.0f, 1));
					break;
				case JUMP:
					jumpList.at(rand() % jumpList.size())(gameObjects, glm::vec3(xPos, 1, zPos), glm::vec3(1, 1, 1), glm::vec4(1.0f, 1.0f, 0, 1));
					break;
				case DUCK:
					duckList.at(rand() % duckList.size())(gameObjects, glm::vec3(xPos, 2, zPos), glm::vec3(1, 1, 1), glm::vec4(1.0f, 0, 1.0f, 1));
					break;
				case NONE:
					continue;
			}
		}
	}

	// Adding in the floor
	AddFloor(gameObjects);

	// Filling the chunk
	std::shared_ptr<GameChunk> chunkPointer = std::make_shared<GameChunk>(gameObjects, glm::vec3(0, 0, Z_THRESHOLD - (CHUNKS_ON_SCREEN * FLOOR_LENGTH)));

	return chunkPointer;
}

ChunkObstacle ChunkGenerator::getObstacleFromInt(int index)
{
	switch (index)
	{
	case 0:
		return NONE;
	case 1:
		return BLOCK;
	case 2:
		return JUMP;
	case 3:
		return DUCK;
	}
}

