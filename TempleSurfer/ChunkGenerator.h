#pragma once
#include "GameChunk.h"
#include <vector>

/*These are the posible settings for obstacels*/
enum class ChunkObstacle {
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
	std::shared_ptr<GameChunk> getChunk();
private:
	std::vector<ChunkPreset> presets;
	std::vector<std::shared_ptr<GameChunk>> chunkPointers;

	std::shared_ptr<GameChunk> buildChunk(ChunkPreset preset);
};