#pragma once
#include "GameChunk.h"
#include <vector>

/*These are the posible settings for obstacels*/
enum ChunkObstacle {
	NONE,
	BLOCK,
	JUMP,
	DUCK
};

/*These are all the posible locations for these settings*/
struct ChunkPreset {
	ChunkObstacle obstacles[3][3];
};

class ChunkGenerator {
public:
	void generatorInit();
	GameChunk generateChunk();
private:
	std::vector<ChunkPreset> presets;
};