#pragma once
#include "GameChunk.h"
#include <vector>

#define MATRIX_SIZE 3
#define CHUNKS_ON_SCREEN 4

/*These are the possible settings for obstacles*/
enum class ChunkObstacle {
	NONE,
	BLOCK,
	JUMP,
	DUCK,
	POWERUP
};

/*These are all the possible locations for these settings*/
struct ChunkPreset {
	ChunkObstacle obstacles[MATRIX_SIZE][MATRIX_SIZE];
};

class ChunkGenerator {
public:
	void generatorInit();
	std::shared_ptr<GameChunk> getChunk();
private:
	std::vector<ChunkPreset> presets;

	bool lastChunkHadPowerup = false;

	std::shared_ptr<GameChunk> buildChunk(ChunkPreset preset);
	ChunkObstacle getObstacleFromInt(int index);
	bool checkForPowerup(int presetInt);
};